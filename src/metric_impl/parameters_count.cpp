#include "metric_impl/parameters_count.hpp"

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
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    extern std::optional<int> extract_number(const std::string &line);

    int func_param_counter = 0;
    int func_definition_line = -1;

    auto filtered_lines = f.ast | std::views::split('\n') |
                          std::views::transform([](const auto &&el) { return std::string(el.begin(), el.end()); }) |
                          std::views::filter([](const auto &str) { return extract_number(str).has_value(); });

    std::ranges::for_each(filtered_lines, [&](const auto &str) {
        int line_num = extract_number(str).value();
        if (str.contains("function_definition")) {
            func_definition_line = line_num;
        } else if (str.contains("identifier") && line_num == func_definition_line) {
            func_param_counter++;
        }
    });

    return func_param_counter;
}

std::string CountParametersMetric::Name() const { return "CountParametersMetric"; }

}  // namespace analyser::metric::metric_impl
