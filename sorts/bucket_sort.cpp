#include<iostream>
#include<vector>
#include<algorithm>

static int randint(int l, int r) { return l + abs(static_cast<int>(rand())) % (r - l + 1); }

void sort_for_small(std::vector<double>& v);

std::vector<double> bucket_sort(const std::vector<double>& v, size_t block);

auto main() -> signed {
    size_t t; std::cin >> t;
    for (size_t it = 1; it <= t; ++it) {
        size_t n = 1 + static_cast<size_t>(abs(rand()) % 100);
        std::vector<double> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = static_cast<double>(randint(0, 100)) / 100;
        auto res = bucket_sort(v, 10);
        if (!std::is_sorted(res.begin(), res.end())) {
            for (double i : res) std::cout << i << ' ';
            std::cout << std::endl;
            break;
        }
        std::cerr << "OK " << it << '\n';
    }
}

void sort_for_small(std::vector<double>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        auto el = v[i];
        auto max = -1e9;
        for (size_t j = i + 1; j < v.size(); ++j) {
            el = std::min(el, v[j]);
            max = std::max(max, v[j]);
        }
        v[i] = el;
        if (max == v[i]) break;
    }
}

std::vector<double> bucket_sort(const std::vector<double>& v, size_t block) {
    std::vector<std::vector<double>> blocks(block + 1);
    auto min = *min_element(v.begin(), v.end());
    auto max = *max_element(v.begin(), v.end());
    for (size_t i = 0; i < v.size(); ++i) {
        size_t pos = static_cast<size_t>(v[i] * block);
        blocks[pos].push_back(v[i]);
    }
    size_t pos = 0;
    std::vector<double> res(v.size());
    for (auto& bl : blocks) {
        sort_for_small(bl);
        for (auto el : bl) res[pos++] = el;
    }
    return res;
}