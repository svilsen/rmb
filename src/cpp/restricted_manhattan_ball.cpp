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

        unsigned __int128 res = 1;
        for (std::uint64_t i = 1; i <= k; i++) {
            unsigned __int128 mult_i = static_cast<unsigned __int128>((n - k + i) / i); 
            res = res * (n - k + i) / i;
        }

        return static_cast<std::uint64_t>(res);
    }

    //
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

    //
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

    //
    void create_md(
        std::uint64_t & _md,
        const std::uint64_t & d,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        for (std::uint64_t i = 0; i < d; i++) {
            std::uint64_t a = km[i] + c[i];
            std::uint64_t b = kp[i] - c[i];

            const std::uint64_t m = std::max(a, b);
            _md += m;
        }
    }

    //
    void create_md_ab(
        std::uint64_t & _md,
        std::vector<std::uint64_t> & _a,
        std::vector<std::uint64_t> & _b,
        const std::uint64_t & d,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        for (std::uint64_t i = 0; i < d; i++) {
            _a[i] = km[i] + c[i];
            _b[i] = kp[i] - c[i];

            const std::uint64_t m = std::max(_a[i], _b[i]);
            _md += m;
        }
    }

    ////
    //
    struct Choice {
        std::uint64_t s;
        std::uint64_t cost;
        std::uint64_t weight;
    };

    void create_grp(
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & _groups,
        std::uint64_t & _md,
        const std::uint64_t & d,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        for (std::uint64_t i = 0; i < d; i++) {
            std::uint64_t a = km[i] + c[i];
            std::uint64_t b = kp[i] - c[i];

            _groups[{std::min(a, b), std::max(a, b)}]++;
            _md += std::max(a, b);
        }
    }

    void choices_grp(
        std::vector<std::vector<Choice>> & _choices,
        const std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint64_t> & groups,
        const std::uint64_t & r
    ) {
        for (const auto & [ab, m] : groups) {
            auto [a, b] = ab;

            std::vector<Choice> choices;
            for (std::uint64_t t = 0; t <= m; t++) {
                for (std::uint64_t u = 0; u <= m - t; u++) {
                    unsigned __int128 q = static_cast<unsigned __int128>(t) * (a + 1) + static_cast<unsigned __int128>(u) * (b + 1);

                    if (q > r) {
                        continue;
                    }

                    unsigned __int128 qp = static_cast<unsigned __int128>(choose(m, t)) * static_cast<unsigned __int128>(choose(m - t, u));

                    Choice grp_tu{t + u, static_cast<std::uint64_t>(q), static_cast<std::uint64_t>(qp)};
                    choices.push_back(grp_tu);
                }
            }

            _choices.push_back(choices);
        }
    }

    ////
    //
    std::uint64_t bruteforce(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        std::vector<std::int64_t> low(d), high(d), u(d);
        for (std::uint64_t i = 0; i < d; i++) {
            low[i] = -static_cast<std::int64_t>(km[i]);
            high[i] = static_cast<std::int64_t>(kp[i]);
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

    ////
    //
    std::uint64_t inner_sum(
        const std::uint64_t & d,
        const std::uint64_t & s,
        const std::uint64_t & n
    ) {
        //
        const unsigned __int128 i_max = static_cast<unsigned __int128>(std::min(d - s, n));

        //
        unsigned __int128 c1 = 1;
        unsigned __int128 c2 = static_cast<unsigned __int128>(choose(n + d, d));

        //
        unsigned __int128 inner = 0;
        for (unsigned __int128 i = 0; i <= i_max; i++) {
            inner += c1 * c2;
            if (i < i_max) {
                c1 = c1 * (d - s - i) / (i + 1);
                c2 = c2 * (n - i) / (n - i + d);
            }
        }

        return static_cast<std::uint64_t>(inner);
    }

    //
    std::int64_t exact(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::uint64_t md = 0;
        create_md(md, d, km, kp, c);

        //
        std::uint64_t r_adj = std::min(r, md);

        //
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

                std::int64_t n = r_st(r_adj, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                std::uint64_t inner = inner_sum(d, s, static_cast<std::uint64_t>(n));
                if (sign > 0) {
                    total += static_cast<std::int64_t>(inner);
                }
                else {
                    total -= static_cast<std::int64_t>(inner);
                }
            }
        }

        //
        std::uint64_t utotal = 0;
        if (total >= 0) {
            utotal = static_cast<std::uint64_t>(total);
        }

        //
        return utotal;
    }

    //
    std::int64_t exact_grp(
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
        create_grp(groups, md, d, km, kp, c);

        //
        std::uint64_t r_adj = std::min(r, md);

        //
        std::vector<std::vector<Choice>> choices;
        choices_grp(choices, groups, r_adj);

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
            for (std::size_t i = 0; i < indices.size(); i++) {
                Choice & choice = choices[i][indices[i]];
                s += choice.s;
                cost += choice.cost;
                weight *= choice.weight;
            }

            //
            std::int64_t sign = (s % 2) ? -1 : 1;
            if (cost <= r_adj) {
                std::uint64_t n = r_adj - cost;
                std::uint64_t inner = inner_sum(d, s, n);

                unsigned __int128 contribution = static_cast<unsigned __int128>(weight) * static_cast<unsigned __int128>(inner);
                total += sign * static_cast<std::int64_t>(contribution);
            }

            //
            for (std::size_t i = 0; i < indices.size(); i++) {
                if (++indices[i] < choices[i].size()) {
                    break;
                }

                indices[i] = 0;
                if (i == indices.size() - 1) {
                    done = true;
                }
            }
        }

        //
        std::uint64_t utotal = 0;
        if (total >= 0) {
            utotal = static_cast<std::uint64_t>(total);
        }

        //
        return utotal;
    }

    ////
    //
    std::uint64_t dp(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::vector<std::uint64_t> a(d), b(d);
        std::uint64_t md = 0;
        create_md_ab(md, a, b, d, km, kp, c);

        //
        const std::uint64_t r_adj = std::min(r, md);
        std::vector<std::uint64_t> crt(r_adj + 1, 0);
        std::vector<std::uint64_t> nxt(r_adj + 1, 0);

        //
        crt[0] = 1;
        std::uint64_t rch = 0;
        for (std::uint64_t i = 0; i < d; i++) {
            rch += std::max(a[i], b[i]);
            const std::uint64_t jmax = std::min(r_adj, rch);

            std::uint64_t wa = 0, wb = 0;
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
        }

        std::uint64_t count = 0;
        for (const std::uint64_t x : crt) {
            count += x;
        }

        return count;
    }

    //
    std::uint64_t dp_grp(
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
        create_grp(groups, md, d, km, kp, c);

        //
        const std::uint64_t r_adj = std::min(r, md);
        std::vector<std::uint64_t> crt(r_adj + 1, 0);
        std::vector<std::uint64_t> nxt(r_adj + 1, 0);

        //
        crt[0] = 1;
        std::uint64_t rch = 0;
        for (const auto & [ab, multiplicity] : groups) {
            ////
            //
            const auto [a, b] = ab;
            for (std::uint64_t k = 0; k < multiplicity; k++) {
                ////
                //
                rch += std::max(a, b);
                const std::uint64_t jmax = std::min(r_adj, rch);

                //
                std::uint64_t wa = 0, wb = 0;
                for (std::uint64_t j = 0; j <= jmax; j++) {
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

    ////
    //
    std::pair<uint64_t, double> approximate(
        const std::uint64_t & d,
        const std::uint64_t & r,
        const std::vector<std::uint64_t> & km,
        const std::vector<std::uint64_t> & kp,
        const std::vector<std::int64_t> & c
    ) {
        ////
        //
        std::uint64_t md = 0;
        create_md(md, d, km, kp, c);

        //
        std::uint64_t r_adj = std::min(r, md);

        //
        std::int64_t estimate = 0;
        double bound = 0;
        for (int Sbits = 0; Sbits < (1 << d); Sbits++) {
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

                std::int64_t n = r_st(r_adj, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                std::uint64_t lt = static_cast<std::uint64_t>(1ULL << (d - s)) * choose(n + d, d);
                estimate += sign * static_cast<std::int64_t>(lt);
                bound += static_cast<double>(lt) * d * (d - s) / (2.0 * (n + 1));
            }
        }

        std::uint64_t uestimate = 0;
        if (estimate >= 0) {
            uestimate = static_cast<std::uint64_t>(estimate);
        }

        return {uestimate, bound};
    }

    //
    std::pair<std::uint64_t, double> approximate_grp(
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
        create_grp(groups, md, d, km, kp, c);

        //
        std::uint64_t r_adj = std::min(r, md);

        //
        std::vector<std::vector<Choice>> choices;
        choices_grp(choices, groups, r_adj);

        ////
        //
        std::vector<std::size_t> indices(groups.size(), 0);

        //
        std::int64_t estimate = 0;
        double bound = 0.0;

        //
        bool done = false;
        while (!done) {
            ////
            //
            std::uint64_t s = 0, cost = 0, weight = 1;
            for (std::size_t i = 0; i < indices.size(); i++) {
                Choice & choice = choices[i][indices[i]];
                s += choice.s;
                cost += choice.cost;
                weight *= choice.weight;
            }

            //
            std::int64_t sign = s % 2 == 0 ? 1 : -1;
            if (cost <= r_adj) {
                std::uint64_t n = r_adj - cost;
                std::uint64_t lt = (std::uint64_t{1} << (d - s)) * choose(n + d, d);
                std::uint64_t contribution = weight * lt;

                estimate += sign * static_cast<std::int64_t>(contribution);
                bound += static_cast<double>(contribution) * d * (d - s) / (2.0 * (n + 1));
            }

            //
            for (std::uint64_t i = 0; i < indices.size(); i++) {
                if (indices[i]++ < choices[i].size()) {
                    break;
                }

                indices[i] = 0;
                if (i == indices.size() - 1) {
                    done = true;
                }
            }
        }

        //
        std::uint64_t uestimate = 0;
        if (estimate >= 0) {
            uestimate = static_cast<std::uint64_t>(estimate);
        }

        //
        return {uestimate, bound};
    }
}
