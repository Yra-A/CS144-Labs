#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , _rto(retx_timeout) {}

uint64_t TCPSender::bytes_in_flight() const { return _bytes_in_flight; }

void TCPSender::fill_window() {
    if (_fin) { // 已经结束了
        return;
    };

    if (_syn == false) { // 如果这是第一个报文段(不存数据)
        TCPSegment seg;
        seg.header().syn = true; // 设置 syn 比特位
        _syn = true;
        send_segment(seg);
        return;
    }

    size_t win = _window_size;
    if (win == 0) win = 1;

    // 没有数据了，且此时有空间
    if (stream_in().eof() && send_base + win > _next_seqno) {
        TCPSegment seg;
        seg.header().fin = true;
        _fin = true;
        send_segment(seg);
        return;
    }

    // 还有数据的话，且此时有空间
    size_t remain = 0; // 计算剩余空间，即 next_seqno 到右端点(send_base + win)的距离
    while ((remain = win - (_next_seqno - send_base)) > 0 && !stream_in().buffer_empty() && _fin == false) { // 具体计算的示意图可以看自顶向下书上的图
        TCPSegment seg;
        size_t size = std::min(TCPConfig::MAX_PAYLOAD_SIZE, remain); // 不能超过上限值
        seg.payload() = _stream.read(std::min(size, _stream.buffer_size()));

        // 设置 FIN 位
        if (stream_in().eof() && seg.length_in_sequence_space() + 1 <= remain) { // fin 位也会占据一个序号
            _fin = true;
            seg.header().fin = true;
        }

        if (seg.length_in_sequence_space() == 0) {
            return;
        }
        send_segment(seg);
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    size_t abs_ackno = unwrap(ackno, _isn, send_base);
    if (abs_ackno > _next_seqno) return;

    _window_size = window_size;

    if (abs_ackno <= send_base) return; // 已接收过的 ack
    send_base = abs_ackno;

    while (!_outstanding.empty()) {
        auto &seg = _outstanding.front();
        // size_t first_seqn = unwrap(seg.header().seqno, _isn, send_base);
        size_t first_seqn = unwrap(seg.header().seqno, _isn, send_base);
        if (first_seqn + seg.length_in_sequence_space() <= send_base) {
            _bytes_in_flight -= seg.length_in_sequence_space();
            _outstanding.pop();
        } else {
            break;
        }
    }

    _rto = _initial_retransmission_timeout; // 重置 RTO
    _consecutive_retransmissions = 0; // 重置连续重传次数
    fill_window(); // 尽可能发送
    if (_outstanding.size()) { // 如果还有别的已发送未确认报文段就重启计时器
        _timer_running = true;
        _timer = 0;
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    _timer += ms_since_last_tick;
    if (_outstanding.empty()) {_timer_running = false;  // 关闭计时器
        return;
    }
        
    if (_timer >= _rto) {
        _segments_out.emplace(_outstanding.front()); // 重传最早的那个段
        _timer = 0; // 重置计时器
        _timer_running = true; // 开启计时器
        if (_window_size > 0 || _outstanding.front().header().syn) { // 需要满足接收窗口非零
            _rto *= 2;
            _consecutive_retransmissions += 1;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _consecutive_retransmissions; }

void TCPSender::send_empty_segment() { // 无 payload, SYN, or FIN
    TCPSegment seg;
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.emplace(seg);
}

void TCPSender::send_segment(TCPSegment &seg) {
    seg.header().seqno = wrap(_next_seqno, _isn);
    _bytes_in_flight += seg.length_in_sequence_space();
    _next_seqno += seg.length_in_sequence_space();
    _outstanding.emplace(seg);
    _segments_out.emplace(seg);

    if (_timer_running == false) { // 开启计时器
        _timer_running = true;
        _timer = 0;
    }
}
