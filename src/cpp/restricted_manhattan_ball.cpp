#include <vector>
#include <bits/stdc++.h>
#include <rmb/restricted_manhattan_ball.hpp>

namespace rmb {
    ////
    //
    unsigned long long choose(int n, int k) {
        if (k < 0 || k > n) {
            return 0;
        }

        if (k > n - k) {
            k = n - k;
        }

        unsigned long long res = 1;
        for (int i = 1; i <= k; i++) {
            res = res * (n - k + i) / i;
        }

        return res;
    }

    unsigned long long delannoy(int m, int n) {
        if (n < 0) {
            return 0;
        }

        unsigned long long s = 0;
        for (int i = 0; i <= std::min(m, n); ++i) {
            s += (1ULL << i) * choose(m, i) * choose(n, i);
        }

        return s;
    }

    ////
    //
    int r_st(const int & r, const std::vector<int> & km, const std::vector<int> & kp, const std::vector<int> & c, const std::vector<int> & S, const std::vector<int> & T) {
        int n = r;
        for (int i : T) {
            n -= km[i] + c[i] + 1;
        }

        for (int i : S) {
            if (find(T.begin(), T.end(), i) == T.end()) {
                n -= kp[i] - c[i] + 1;
            }
        }

        return n;
    }

    //
    unsigned long long bruteforce(const int & d, const int & r, const std::vector<int>& km, const std::vector<int>& kp, const std::vector<int>& c) {
        std::vector<int> low(d), high(d), u(d);
        for (int i = 0; i < d; i++) { 
            low[i] = -km[i];
            high[i] = kp[i];
            u[i] = low[i];
        }

        unsigned long long cnt = 0;
        while (true) {
            int dist = 0;
            for (int i = 0; i < d; i++) {
                dist += abs(u[i] - c[i]);
            }

            if (dist <= r) {
                cnt++;
            }

            int i = d - 1; 
            while (i >= 0 && ++u[i] > high[i]) { 
                u[i] = low[i];
                i--;
            }

            if (i < 0) {
                break;
            }
        }

        return cnt;
    }

    //
    long long exact(int & d, int & r, const std::vector<int> & km, const std::vector<int> & kp, const std::vector<int> & c) {
        long long total = 0;
        for (int Sbits = 0; Sbits < (1 << d); Sbits++) {
            std::vector<int> S;
            for (int i = 0; i < d; i++) {
                if (Sbits >> i & 1) {
                    S.push_back(i);
                }
            }

            int s = (int)S.size();
            long long sign = (s % 2) ? -1 : 1;
            for (int Tbits = 0; Tbits < (1 << s); Tbits++) {
                std::vector<int> T;
                for (int i = 0; i < s; i++) {
                    if (Tbits >> i & 1) {
                        T.push_back(S[i]);
                    }
                }

                int n = r_st(r, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                unsigned long long inner = 0;
                for (int j = 0; j <= n; j++) {
                    unsigned long long coef = (s == 0) ? (j == 0 ? 1 : 0) : choose(j + s - 1, s - 1);
                    inner += coef * delannoy(d - s, n - j);
                }

                total += sign * (long long)inner;
            }
        }

        return total;
    }

    //
    std::pair<long long, double> approximate(int & d, const int & r, const std::vector<int>& km, const std::vector<int>& kp, const std::vector<int>& c) {
        long long est = 0;
        double bound = 0;
        for (int Sbits = 0; Sbits < (1 << d); Sbits++) {
            std::vector<int> S;
            for (int i = 0; i < d; ++i) {
                if (Sbits >> i & 1) {
                    S.push_back(i);
                }
            }

            int s = (int)S.size();
            long long sign = (s % 2) ? -1 : 1;
            for (int Tbits = 0; Tbits < (1 << s); Tbits++) {
                std::vector<int> T;
                for (int i = 0; i < s; ++i) {
                    if (Tbits >> i & 1) {
                        T.push_back(S[i]);
                    }
                }

                int n = r_st(r, km, kp, c, S, T);
                if (n < 0) {
                    continue;
                }

                long long lt = (long long)(1ULL << (d - s)) * (long long) choose(n + d, d);
                est += sign * lt;
                bound += (double)lt * d * (d - s) / (2.0 * (n + 1));
            }
        }

        return {est, bound};
    }
}
