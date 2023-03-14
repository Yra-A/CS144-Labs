#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // uint32_t ISN = isn.raw_value();
    // uint32_t MOD = ~0u;
    // n = (n + ISN) % MOD;
    // return WrappingInt32{static_cast<uint32_t>(n)};
    return WrappingInt32{static_cast<uint32_t>(n) + isn.raw_value()};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t down = checkpoint & 0xFFFFFFFF00000000; // 所属周期的最小值
    uint64_t offset = n.raw_value() - isn.raw_value(); // 偏移量
    uint64_t m = down + offset; // checkpoint 所属周期中的一个解
    uint64_t P = 1uL << 32; // 周期长度
    uint64_t res = m; 
    uint64_t dif = (checkpoint >= res) ? checkpoint - res : res - checkpoint; // 当前解的差
    // 下面是对下一个区间和上一个区间中的解进行判断是否更优，需要注意不要数据上溢和下溢
    if (m + P > P && (m + P - checkpoint) < dif) {
        dif = m + P - checkpoint;
        res = m + P;
    }
    if (m >= P && checkpoint - (m - P) < dif) {
        res = m - P;
    }
    return res;
}
