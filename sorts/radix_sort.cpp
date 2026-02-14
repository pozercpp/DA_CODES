#include<iostream>
#include<vector>
#include<algorithm>
#include<ranges>

template<class T>
std::vector<T> count_sort(const std::vector<T>& v);

template<class T>
std::vector<T> radix_sort(const std::vector<T>& v);

static int randint(int l, int r) { return l + abs(static_cast<int>(rand())) % (r - l + 1); }

auto main() -> signed {
    size_t t; std::cin >> t;
    for (size_t it = 1; it <= t; ++it) {
        size_t n = 1 + static_cast<size_t>(abs(rand()) % 100000);
        std::vector<int> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = randint(-1e9, 1e9);
        auto res = radix_sort(v);
        if (!std::is_sorted(res.begin(), res.end())) {
            for (int i : res) std::cout << i << ' ';
            std::cout << std::endl;
            break;
        }
        std::cerr << "OK " << it << '\n';
    }
}

template<class T>
std::vector<T> count_sort(const std::vector<T>& v, size_t bit, T min) {
    std::vector<T> res(v.size());
    std::vector<int> cnt(256);
    for (size_t i = 0; i < v.size(); ++i) {
        unsigned long long val = static_cast<unsigned long long>(v[i] - min);
        ++cnt[(val >> bit) & 255];
    }
    for (size_t i = 0; i + 1 < cnt.size(); ++i) cnt[i + 1] += cnt[i];
    for (int i = v.size() - 1; i >= 0; --i) {
        unsigned long long val = static_cast<unsigned long long>(v[i] - min);
        --cnt[(val >> bit) & 255];
        res[cnt[(val >> bit) & 255]] = v[i];
    }
    return res;
}

template<class T>
size_t calc_iters(T n) {
    size_t bit = 1;
    while (n) {
        ++bit;
        n >>= 8;
    }
    return bit;
}

template<class T>
std::vector<T> radix_sort(const std::vector<T>& v) {
    auto min = std::ranges::min_element(v);
    auto max = std::ranges::max_element(v);
    size_t bit = 0;
    auto iters = calc_iters(max - min);
    auto res = v;
    for (size_t it = 0; it <= iters; ++it) {
        res = count_sort(res, bit, min);
        bit += 8;
    }
    return res;
}
