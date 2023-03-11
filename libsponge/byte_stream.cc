#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : cap(capacity), have_written(0), have_read(0), buf{}, write_end(false) { }// 构造函数初始化

size_t ByteStream::write(const string &data) {
    int cnt = 0; // 写入了多少字节
    for (auto x : data) {
        if (buf.size() >= cap) break; // 保证不超过 capacity
        buf += x;
        have_written += 1;
        cnt += 1;
    }
    return cnt;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    return std::string(buf.begin(), buf.begin() + std::min(buf.size(), len));
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (len > buf.size()) {
        set_error(); // 设置 error
        return;
    }
    buf.erase(0, len);
    have_read += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    if (len > buf.size()) {
        set_error(); // 设置 error
        return "";
    }
    std::string res = std::string(buf.begin(), buf.begin() + len);
    buf.erase(0, len);
    have_read += len;
    return res;
}

void ByteStream::end_input() {
    write_end = true;
}

bool ByteStream::input_ended() const {
    return write_end;
}

size_t ByteStream::buffer_size() const {
    return buf.size();
}

bool ByteStream::buffer_empty() const {
    return buf.empty();   
}

bool ByteStream::eof() const {
    return buf.empty() && write_end;
}

size_t ByteStream::bytes_written() const {
    return have_written;
}

size_t ByteStream::bytes_read() const {
    return have_read;
}

size_t ByteStream::remaining_capacity() const {
    return cap - buf.size();
}
