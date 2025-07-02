#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    auto [metric_element, is_inserted] = categories_freq.insert({metric_result.metric_name, metric_result.value});

    if (is_inserted) {
        categories_data_counter[metric_result.metric_name] = 1.;
    } else {
        metric_element->second += metric_result.value;
        categories_data_counter[metric_result.metric_name]++;
    }

    is_finalized = false;
}

const std::unordered_map<std::string, double> &CategoricalAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("Result is not finalized!");
    }
    return categories_freq;
};

void CategoricalAccumulator::Finalize() {
    std::ranges::for_each(categories_freq, [&](auto &pair) {
        auto &[category, freq] = pair;
        auto it = categories_data_counter.find(category);
        if (it != categories_data_counter.end() && it->second != 0) {
            freq /= it->second;
        } else {
            freq = 0;
        }
    });
    is_finalized = true;
}

void CategoricalAccumulator::Reset() {
    categories_freq.clear();
    categories_data_counter.clear();
    is_finalized = false;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
