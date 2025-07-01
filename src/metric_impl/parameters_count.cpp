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
#include <print>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    extern std::optional<int> ExtractCodeLineNumber(const std::string_view line);

    int func_param_counter = 0;

    auto filtered_lines = f.ast | std::views::split('\n') |
                          std::views::transform([](const auto &&el) { return std::string(el.begin(), el.end()); }) |
                          std::views::filter([](const auto &str) { return ExtractCodeLineNumber(str).has_value(); });

    std::ranges::for_each(filtered_lines, [&](const auto &str) {
        static int func_definition_line = -1;
        static int parameters_spaces_count = 0;  // spaces count for function parameters line
        int line_num = ExtractCodeLineNumber(str).value();

        int spaces_count = std::distance(str.begin(), std::ranges::find_if(str, [](char c) { return c != ' '; }));
        if (str.contains("function_definition")) {
            func_definition_line = line_num;
            parameters_spaces_count = 0;
        } else if (str.contains("parameters") && line_num == func_definition_line && !parameters_spaces_count) {
            parameters_spaces_count = spaces_count + 2;  // we have 2 more spaces after parameters definition
        } else if ((str.contains("identifier") || str.contains("typed_parameter") ||
                    str.contains("typed_default_parameter ") || str.contains("default_parameter ") ||
                    str.contains("list_splat_pattern ") || str.contains("dictionary_splat_pattern ")) &&
                   line_num == func_definition_line && spaces_count == parameters_spaces_count) {
            func_param_counter++;
        }
    });

    return func_param_counter;
}

std::string CountParametersMetric::Name() const { return "CountParametersMetric"; }

}  // namespace analyser::metric::metric_impl
