#pragma once
#include <vector>

namespace rmb {
    unsigned long long choose(int n, int k);
    unsigned long long delannoy(int m, int n);

    int r_st(const int & r, const std::vector<int> & km, const std::vector<int> & kp, const std::vector<int> & c, const std::vector<int> & S, const std::vector<int> & T);

    unsigned long long bruteforce(const int & d, const int & r, const std::vector<int>& km, const std::vector<int>& kp, const std::vector<int>& c);
    long long exact(int & d, int & r, const std::vector<int> & km, const std::vector<int> & kp, const std::vector<int> & c);
    std::pair<long long, double> approximate(int & d, const int & r, const std::vector<int>& km, const std::vector<int>& kp, const std::vector<int>& c);
}
