#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {

    TCPHeader head = seg.header(); // 获取头部

    std::string data = seg.payload().copy();

    if (syn == false && head.syn == false) {
        return;
    }

    bool eof = false;

    if (syn == false && head.syn == true) { // 如果这是初始段
        syn = true;
        isn = head.seqno;
        if (fin == false && head.fin == true) { // 这也是末尾段
            eof = fin = true;
        }
        _reassembler.push_substring(data, 0, eof);
        return;
    }

    if (head.fin == true) {
        eof = fin = true;
    }

    _reassembler.push_substring(
        data,
        unwrap(head.seqno, isn, _reassembler.exp()) - syn, // syn 会占据一个 sequence number
        eof
    );
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (syn) { // 接受过数据
        return wrap(_reassembler.exp() + syn + (_reassembler.empty() && fin), WrappingInt32(isn)); // syn 和 fin 都会占据一个 sequence number
    }
    return std::nullopt;
}

size_t TCPReceiver::window_size() const {
    return _capacity - stream_out().buffer_size();
}
