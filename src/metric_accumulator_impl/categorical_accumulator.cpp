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
        categories_data_counter[metric_result.metric_name] = 1;
    } else {
        double n = static_cast<double>(categories_data_counter.at(metric_result.metric_name));

        metric_element->second = metric_element->second * (n / (n + 1)) + (metric_result.value / (n + 1));
        categories_data_counter[metric_result.metric_name]++;
    }
}

void CategoricalAccumulator::Reset() {
    categories_freq.clear();
    categories_data_counter.clear();
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
