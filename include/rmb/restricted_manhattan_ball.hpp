#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace rmb {
    ////
    //
    std::uint64_t choose(
        const std::uint64_t & n, 
        std::uint64_t k
    );

    //
    std::uint64_t delannoy(
        const std::uint64_t & m, 
        const std::uint64_t & n
    );

    //
    std::int64_t r_st(
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c,
        const std::vector<std::uint64_t> & S,
        const std::vector<std::uint64_t> & T
    );

    ////
    //
    void create_grp(
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & _groups,
        std::uint64_t & _md,
        const std::uint64_t & d,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    void choices_grp(
        std::vector<std::vector<std::uint64_t>> & _s,
        std::vector<std::vector<std::uint64_t>> & _costs,
        std::vector<std::vector<std::uint64_t>> & _weights,
        const std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & groups,
        const std::uint64_t & r
    );

    ////
    //
    std::uint64_t bruteforce(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    ////
    //
    std::uint64_t inner_sum(
        const std::uint64_t & d,
        const std::uint64_t & s,
        const std::uint64_t & n
    );

    //
    std::int64_t exact(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    //
    std::int64_t exact_grp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    ////
    //
    std::uint64_t dp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    //
    std::uint64_t dp_grp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    ////
    //
    std::pair<uint64_t, double> approximate(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );

    //
    std::pair<std::uint64_t, double> approximate_grp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    );
}
