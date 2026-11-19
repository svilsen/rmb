#include <algorithm>
#include <cstdint>
#include <vector>
#include <map>

#include <rmb/restricted_manhattan_ball.hpp>

namespace rmb {
    ////
    //
    std::uint64_t choose(
        const std::uint64_t & n, 
        std::uint64_t k
    ) {
        if (k > n) {
            return 0;
        }

        if (k > n - k) {
            k = n - k;
        }

        std::uint64_t res = 1;
        for (std::uint64_t i = 1; i <= k; ++i) {
            res = res * (n - k + i) / i;
        }

        return res;
    }

    std::uint64_t delannoy(
        const std::uint64_t & m, 
        const std::uint64_t & n
    ) {
        std::uint64_t s = 0;
        for (std::uint64_t i = 0; i <= std::min(m, n); i++) {
            s += (std::uint64_t{1} << i) * choose(m, i) * choose(n, i);
        }

        return s;
    }

    std::int64_t r_st(
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c,
        const std::vector<std::uint64_t> & S,
        const std::vector<std::uint64_t> & T
    ) {
        std::int64_t n = r;
        for (std::uint64_t i : T) {
            n -= static_cast<std::int64_t>(km[i]) + c[i] + 1;
        }

        for (std::uint64_t i : S) {
            if (std::find(T.begin(), T.end(), i) == T.end()) {
                n -= static_cast<std::int64_t>(kp[i]) - c[i] + 1;
            }
        }

        return n;
    }

    std::uint64_t bruteforce(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        std::vector<std::int64_t> low(d), high(d), u(d);
        for (std::uint64_t i = 0; i < d; i++) {
            low[i] = -km[i];
            high[i] = kp[i];
            u[i] = low[i];
        }

        std::uint64_t cnt = 0;
        while (true) {
            std::int64_t dist = 0;
            for (std::uint64_t i = 0; i < d; i++) {
                if (u[i] > c[i]) {
                    dist += u[i] - c[i];
                }
                else {
                    dist += c[i] - u[i];
                }
            }

            if (dist <= r) {
                cnt++;
            }

            std::uint64_t i = d - 1;
            while (true) {
                if (u[i] < high[i]) {
                    u[i]++;
                    break;
                }

                u[i] = low[i];
                if (i == 0) {
                    break;
                }

                i--;
            }

            if (i == 0 && u[i] == low[i]) {
                break;
            }
        }

        return cnt;
    }

    std::int64_t exact(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        std::int64_t total = 0;
        for (std::uint64_t Sbits = 0; Sbits < (std::uint64_t{1} << d); Sbits++) {
            std::vector<std::uint64_t> S;
            for (std::uint64_t i = 0; i < d; i++) {
                if (Sbits >> i & 1) {
                    S.push_back(i);
                }
            }

            std::uint64_t s = static_cast<std::uint64_t>(S.size());
            std::int64_t sign = (s % 2) ? -1 : 1;
            for (std::uint64_t Tbits = 0; Tbits < (std::uint64_t{1} << s); Tbits++) {
                std::vector<std::uint64_t> T;
                for (std::uint64_t i = 0; i < s; i++) {
                    if (Tbits >> i & 1) {
                        T.push_back(S[i]);
                    }
                }

                std::int64_t n = r_st(r, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                std::uint64_t inner = 0;
                for (std::uint64_t j = 0; j <= n; j++) {
                    std::uint64_t coef = (s == 0) ? (j == 0 ? 1 : 0) : choose(j + s - 1, s - 1);
                    inner += coef * delannoy(d - s, n - j);
                }

                if (sign > 0) {
                    total += static_cast<int64_t>(inner);
                }
                else {
                    total -= static_cast<int64_t>(inner);
                }
            }
        }

        return total;
    }

    std::uint64_t dp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {

        std::vector<std::int64_t> a(d), b(d);
        std::uint64_t md = 0;
        for (std::uint64_t i = 0; i < d; ++i) {
            a[i] = static_cast<std::int64_t>(km[i]) + c[i];
            b[i] = static_cast<std::int64_t>(kp[i]) - c[i];

            const std::uint64_t m = std::max(a[i], b[i]);
            md += m;
        }

        const std::uint64_t r_adj = std::min(r, md);
        std::vector<std::uint64_t> crt(r_adj + 1, 0);
        std::vector<std::uint64_t> nxt(r_adj + 1, 0);

        crt[0] = 1;
        for (std::uint64_t i = 0; i < d; i++) {
            std::uint64_t wa = 0;
            std::uint64_t wb = 0;
            for (std::uint64_t j = 0; j <= r_adj; j++) {
                if (j > 0) {
                    if (j > a[i]) {
                        wa -= crt[j - a[i] - 1];
                    }

                    wa += crt[j - 1];
                }

                if (j > 0) {
                    if (j > b[i]) {
                        wb -= crt[j - b[i] - 1];
                    }

                    wb += crt[j - 1];
                }

                nxt[j] = crt[j] + wa + wb;
            }

            crt.swap(nxt);
            std::fill(nxt.begin(), nxt.end(), 0);
        }

        std::uint64_t count = 0;
        for (const std::uint64_t x : crt) {
            count += x;
        }

        return count;
    }

    //
    std::pair<uint64_t, double> approximate(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        uint64_t estimate = 0;
        double bound = 0;
        for (int Sbits = 0; Sbits < (1 << d); Sbits++) {
            std::vector<uint64_t> S;
            for (int i = 0; i < d; ++i) {
                if (Sbits >> i & 1) {
                    S.push_back(i);
                }
            }

            std::uint64_t s = static_cast<std::uint64_t>(S.size());
            std::int64_t sign = (s % 2) ? -1 : 1;
            for (std::uint64_t Tbits = 0; Tbits < (std::uint64_t{1} << s); Tbits++) {
                std::vector<std::uint64_t> T;
                for (std::uint64_t i = 0; i < s; i++) {
                    if (Tbits >> i & 1) {
                        T.push_back(S[i]);
                    }
                }

                std::int64_t n = r_st(r, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                uint64_t lt = (uint64_t)(1ULL << (d - s)) * choose(n + d, d);
                estimate += sign * lt;
                bound += (double)lt * d * (d - s) / (2.0 * (n + 1));
            }
        }

        return {estimate, bound};
    }


    void groups_create(
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & _groups,
        std::uint64_t & _md,
        const std::uint64_t & d,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        for (std::uint64_t i = 0; i < d; ++i) {
            std::uint64_t a = km[i] + c[i];
            std::uint64_t b = kp[i] - c[i];

            _groups[{a, b}]++;
            _md += std::max(a, b);
        }
    }

    void group_choices(
        std::vector<std::vector<std::uint64_t>> & _s,
        std::vector<std::vector<std::uint64_t>> & _costs,
        std::vector<std::vector<std::uint64_t>> & _weights,
        const std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & groups,
        const std::uint64_t & r
    ) {
        for (const auto & [ab, m] : groups) {
            auto [a, b] = ab;

            std::vector<std::uint64_t> s, cost, weight;
            for (std::uint64_t t = 0; t <= m; ++t) {
                for (std::uint64_t u = 0; u <= m - t; ++u) {
                    unsigned __int128 q =
                        static_cast<unsigned __int128>(t) * (a + 1) +
                        static_cast<unsigned __int128>(u) * (b + 1);

                    if (q > r) continue;

                    s.push_back(t + u);
                    cost.push_back(static_cast<std::uint64_t>(q));
                    weight.push_back(choose(m, t) * choose(m - t, u));
                }
            }

            _s.push_back(std::move(s));
            _costs.push_back(std::move(cost));
            _weights.push_back(std::move(weight));
        }
    }

    std::int64_t grouped_exact(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> groups;
        std::uint64_t md = 0;
        groups_create(groups, md, d, km, kp, c);

        //
        std::vector<std::vector<std::uint64_t>> ss, costs, weights;
        group_choices(ss, costs, weights, groups, r); 
    
        ////
        //
        std::vector<std::size_t> indices(groups.size(), 0);

        //
        std::int64_t total = 0;
        bool done = false;
        while (!done) {
            ////
            //
            std::uint64_t s = 0, cost = 0, weight = 1;
            for (std::size_t i = 0; i < indices.size(); ++i) {
                s += ss[i][indices[i]];
                cost += costs[i][indices[i]];
                weight *= weights[i][indices[i]];
            }

            //
            std::int64_t sign = (s % 2) ? -1 : 1;
            if (cost <= r) {
                std::uint64_t n = r - cost;
                std::uint64_t inner = 0;

                for (std::uint64_t j = 0; j <= n; ++j) {
                    std::uint64_t coef = (s == 0) ? (j == 0 ? 1 : 0) : choose(j + s - 1, s - 1);
                    inner += coef * delannoy(d - s, n - j);
                }

                std::int64_t contribution = static_cast<std::int64_t>(weight * inner);

                total += sign * contribution;
            }

            //
            for (std::size_t i = 0; i < indices.size(); ++i) {
                if (++indices[i] < ss[i].size()) {
                    break;
                }

                indices[i] = 0;
                if (i == indices.size() - 1) done = true;
            }
        }

        return total;
    }

    std::pair<std::uint64_t, double> grouped_approximate(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> groups;
        std::uint64_t md = 0;
        groups_create(groups, md, d, km, kp, c);

        //
        std::vector<std::vector<std::uint64_t>> ss, costs, weights;
        group_choices(ss, costs, weights, groups, r); 

        ////
        //
        std::vector<std::size_t> indices(groups.size(), 0);

        //
        std::uint64_t estimate = 0;
        double bound = 0.0;

        //
        bool done = false;
        while (!done) {
            ////
            //
            std::uint64_t s = 0, cost = 0, weight = 1;
            for (std::size_t i = 0; i < indices.size(); ++i) {
                s += ss[i][indices[i]];
                cost += costs[i][indices[i]];
                weight *= weights[i][indices[i]];
            }

            //
            std::int64_t sign = s % 2 == 0 ? 1 : -1;
            if (cost <= r) {
                std::uint64_t n = r - cost;
                std::uint64_t lt = (std::uint64_t{1} << (d - s)) * choose(n + d, d);
                std::uint64_t contribution = weight * lt;

                estimate += sign * contribution;
                bound += static_cast<double>(contribution) * d * (d - s) / (2.0 * (n + 1));
            }

            //
            for (std::size_t i = 0; i < indices.size(); ++i) {
                if (++indices[i] < ss[i].size()) {
                    break;
                }

                indices[i] = 0;
                if (i == indices.size() - 1) {
                    done = true;
                }
            }
        }

        //
        return {estimate, bound};
    }

    std::uint64_t grouped_dp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> groups;
        std::uint64_t md = 0;
        groups_create(groups, md, d, km, kp, c);

        //
        const std::uint64_t r_adj = std::min(r, md);
        std::vector<std::uint64_t> crt(r_adj + 1, 0);
        std::vector<std::uint64_t> nxt(r_adj + 1, 0);

        //
        crt[0] = 1;
        for (const auto & [ab, multiplicity] : groups) {
            ////
            //
            const auto [a, b] = ab;
            for (std::uint64_t k = 0; k < multiplicity; ++k) {
                ////
                //
                std::uint64_t wa = 0;
                std::uint64_t wb = 0;
                for (std::uint64_t j = 0; j <= r_adj; ++j) {
                    if (j > 0) {
                        if (j > a) {
                            wa -= crt[j - a - 1];
                        }

                        wa += crt[j - 1];
                    }

                    if (j > 0) {
                        if (j > b) {
                            wb -= crt[j - b - 1];
                        }

                        wb += crt[j - 1];
                    }

                    nxt[j] = crt[j] + wa + wb;
                }

                //
                crt.swap(nxt);
                std::fill(nxt.begin(), nxt.end(), 0);
            }
        }

        ////
        //
        std::uint64_t count = 0;
        for (const std::uint64_t x : crt) {
            count += x;
        }

        //
        return count;
    }
}
