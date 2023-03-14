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
    uint32_t ISN = isn.raw_value();
    uint32_t MOD = ~0u;
    n = (n + ISN) % MOD;
    return WrappingInt32{static_cast<uint32_t>(n)};
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
    uint32_t MOD = ~0u;
    uint32_t ISN = isn.raw_value();
    uint32_t N = n.raw_value();
    if (N < ISN) N += MOD;
    uint64_t offset = static_cast<uint64_t>(N - ISN);
    if (N >= checkpoint) return N;
    uint64_t downround = checkpoint / MOD * MOD;
    uint64_t a = downround + offset;
    uint64_t b = a - MOD;
    uint64_t c = a + MOD;
    std::vector<uint64_t> num = {a, b, c};
    uint64_t d1 = a >= checkpoint ? a - checkpoint : checkpoint - a;
    uint64_t d2 = b >= checkpoint ? b - checkpoint : checkpoint - b;
    uint64_t d3 = c >= checkpoint ? c - checkpoint : checkpoint - c;
    std::vector<uint64_t> dif = {d1, d2, d3};

    std::vector<std::pair<uint64_t, uint64_t>> v(3);
    for (int i = 0; i < 3; i++) {
        v[i] = {dif[i], num[i]};    
    }
    std::sort(v.begin(), v.end());
    return v[0].second;
}
