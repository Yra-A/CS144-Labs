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
    if (_active) return;

    _time_since_last_segment_received = 0; // 重置时间

    // 收到 RST 段
    if (seg.header().rst == true) { 
        _sender.stream_in().set_error();
        _receiver.stream_out().set_error();
        _active = true; 
    } 
    // Listening !!!!!!!!!!!!!!!!
    else if (_receiver.ackno().has_value() == false) {
        // 被动连接 Passive open（进入SYN_REVD）
        if (seg.header().syn) {
            _receiver.segment_received(seg);
            connect(); // 会发送 ACK 和 SYN
        }   
    }
    // CLOSED 此时收到 seg 不会有操作 !!!!!!!!!!!!!!!!
    else if (_sender.next_seqno_absolute() == 0) { }
    // SYN_REVD !!!!!!!!!!!!!!!!!!!!!!!
    else if (_receiver.ackno().has_value() == true && _receiver.stream_out().input_ended() == false) { // !!!!!!!!!!!!!!!!!!!!!!
        // sender 接受 ACK（进入ESTABLISHED）
        _receiver.segment_received(seg);
        _sender.ack_received(seg.header().ackno, seg.header().win);
    }
    // SYN_SENT !!!!!!!!!!!!!!!!!!!!
    else if (_sender.next_seqno_absolute() > 0 && _sender.next_seqno_absolute() == _sender.bytes_in_flight()) {
        // client 收到 ACK 和 SYN，发送 ACK 后进入 ESTABLISHED
        if (seg.header().ack == true && seg.header().syn == true) {
            _sender.ack_received(seg.header().ackno, seg.header().win);
            _receiver.segment_received(seg);
            _sender.send_empty_segment(); // 通过空包，来发送 ACK
            // send_data(); // !!!!!!!!!!!!!!!!!!
        }       
        // client 也作为 server，收到了 SYN，发送 SYN 和 ACK 后进入 SYN_REVD
        else if (seg.header().syn == true && seg.header().ack == false) {
            _receiver.segment_received(seg);
            // _sender.send_empty_segment(); // 发送 ACK
            // send_data(); // 发送
            connect(); // !!!!!!!!!!!!!!
        }
    }
    // ESTABLISHED
    else if (_sender.next_seqno_absolute() > _sender.bytes_in_flight() && _sender.stream_in().eof() == false) {
        _sender.ack_received(seg.header().ackno, seg.header().win);
        _receiver.segment_received(seg);
        if (seg.length_in_sequence_space() > 0) {
            _sender.send_empty_segment(); // 发送 ACK
        }
        _sender.fill_window();
        send_data();
    }
    // FIN_WAIT_1（发送完毕，但接受未完毕）
    else if (_sender.stream_in().eof() == true &&
             _sender.next_seqno_absolute() == _sender.stream_in().bytes_written() + 2 && 
             _sender.bytes_in_flight() > 0 &&
             _receiver.stream_out().input_ended() == false)
    { // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // 收到 FIN，进入 CLOSING
        if (seg.header().fin == true && seg.header().ack == false) {
            _sender.ack_received(seg.header().ackno, seg.header().win);
            _receiver.segment_received(seg);
        } 
        // 收到 FIN、ACK，发送 ACK，进入 TIME_WAIT
        else if (seg.header().fin == true && seg.header().ack == true) {
            _sender.ack_received(seg.header().ackno, seg.header().win);
            _receiver.segment_received(seg);
            _sender.send_empty_segment();
            send_data();
        }
        // 收到 ACK，进入 FIN_WAIT_2
        else if (seg.header().fin == false && seg.header().ack == true) {
            _sender.ack_received(seg.header().ackno, seg.header().win);
            _receiver.segment_received(seg);
            send_data(); // !!!!!!!!!!!!!!!!!!!!!!
        }
    }
    // FIN_WAIT_2，收到 FIN，发送 ACK，进入 TIME_WAIT
    else if (_sender.stream_in().eof() == true &&
             _sender.next_seqno_absolute() == _sender.stream_in().bytes_written() + 2 && 
             _sender.bytes_in_flight() == 0 &&
             _receiver.stream_out().input_ended() == false)
    {
        _sender.ack_received(seg.header().ackno, seg.header().win);
        _receiver.segment_received(seg);
        _sender.send_empty_segment();
        send_data();
    }
    // TIME_WAIT
    else if (_sender.stream_in().eof() == true &&
             _sender.next_seqno_absolute() == _sender.stream_in().bytes_written() + 2 && 
             _sender.bytes_in_flight() == 0 &&
             _receiver.stream_out().input_ended() == true)
    {
        // 收到 FIN 的话就发送 ACK（不断开连接）
        if (seg.header().fin == true) {
            _sender.ack_received(seg.header().ackno, seg.header().win);
            _receiver.segment_received(seg);
            _sender.send_empty_segment();
            send_data();
        }
    }
    // 其他状态
    {
        _sender.ack_received(seg.header().ackno, seg.header().win);
        _receiver.segment_received(seg);
        _sender.fill_window();
        send_data();
    }

}

bool TCPConnection::active() const {
    return _active; 
}

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
void TCPConnection::tick(const size_t ms_since_last_tick) {
    if (_active == false) return;

    _time_since_last_segment_received += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);

    if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS) { // 重传次数超过上限
        reset_connection(); // 设置 RST
        return;
    }
    send_data();
}

void TCPConnection::reset_connection() {
    // 发送 RST 段
    TCPSegment seg;
    seg.header().rst = true;
    segments_out().emplace(seg);

    // 标记错误 和 设置 _active
    _sender.stream_in().set_error();
    _receiver.stream_out().set_error();
    _active = true; 
     
}

void TCPConnection::end_input_stream() {
    // 我方发送完毕
    _sender.stream_in().end_input();
    _sender.fill_window();
    send_data();
}

void TCPConnection::connect() {
    // 我方发起连接，第一次 fill_window 会发送 SYN
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
            reset_connection();
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
