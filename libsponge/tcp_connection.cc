#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const {
    return _sender.stream_in().remaining_capacity(); 
}

size_t TCPConnection::bytes_in_flight() const {
    return _sender.bytes_in_flight();
}

size_t TCPConnection::unassembled_bytes() const {
    return _receiver.unassembled_bytes(); 
}

size_t TCPConnection::time_since_last_segment_received() const {
    return _time_since_last_segment_received; 
}

void TCPConnection::segment_received(const TCPSegment &seg) {
DUMMY_CODE(seg);
return;
}

bool TCPConnection::active() const { return _active; }

size_t TCPConnection::write(const string &data) {
    if (data.empty()) {
        return 0;
    }

    _sender.stream_in().write(data); // 往发送方的 bytestream 写入数据
    _sender.fill_window(); // 根据接收窗口发送数据，数据放进 _segments_out
    send_data(); // 发送数据到 receiver

    return data.size();
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) { DUMMY_CODE(ms_since_last_tick); }

void TCPConnection::end_input_stream() {
    // 我方发送完毕
    _sender.stream_in().end_input();
    _sender.fill_window();
    send_data();
}

void TCPConnection::connect() {
    // 我方主动发起连接，第一次 fill_window 会发送 SYN
    _sender.fill_window();
    send_data();
}

void TCPConnection::send_data() {
    // 发送 sender 的信息
    while (!_sender.segments_out().empty()) { // 将 sender 发送队列中的数据放入 connection 的发送队列中
        auto seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        if (_receiver.ackno().has_value()) { // 将自己这里的 receiver 的数据写入首部
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
            seg.header().win = _receiver.window_size() > std::numeric_limits<uint16_t>::max() ? std::numeric_limits<uint16_t>::max() : _receiver.window_size(); // 限定最大值
            
        }
        segments_out().emplace(seg); // 放入 connection 的发送队列
    }

    // 如果对方的发送完毕了
    if (_receiver.stream_out().input_ended()) {
        if (_sender.stream_in().eof() == false) {// 我方的发送还未结束(被动连接)
            _linger_after_streams_finish = false; // 不用进入 TIME_WAIT
        } else if (_sender.stream_in().eof() == true && _sender.bytes_in_flight() == 0) { // 我方已经发送完了，且全送达了
            // 如果我方不用进入 TIME_WAIT 或者 我方 TIME_WAIT 结束
            if (_linger_after_streams_finish == false || _time_since_last_segment_received >= 10 * _cfg.rt_timeout)
            _active = false; // 我方关闭连接 cleanly
        }
    }
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
