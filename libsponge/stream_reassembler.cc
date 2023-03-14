#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), buf{}, expected_num(0), end_flag(false), end_num(0) { }

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t up = expected_num + _capacity - stream_out().buffer_size(); // Reassembler 中能存储的最大字节序号 up
    if (end_flag) {
        up = std::min(up, end_num);
    }
    if (index >= up) return; // 不符合直接 return
    
    for (size_t i = expected_num > index ? expected_num - index : 0; i < data.size() && index + i < up; i++) {
        // if (index + i < expected_num) continue;
        buf[index + i] = data[i];
    }   

    std::string s;
    for (size_t i = expected_num; i < up; i++) { 
        if (buf.count(i) == 0) break;
        s += buf[i];
        expected_num += 1;
        buf.erase(i);
    }

    stream_out().write(s);

    if (eof) {
        end_flag = true;
        end_num = index + data.size();
    }

    if (end_flag && expected_num == end_num) {
        stream_out().end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    return buf.size();
}

bool StreamReassembler::empty() const {
    return buf.empty();
}
