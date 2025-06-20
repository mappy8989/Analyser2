#include "metric_impl/code_lines_count.hpp"

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
#include <print>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

std::optional<int> extract_number(const std::string &line) {
    auto pos1 = line.find('[');
    if (pos1 == std::string::npos)
        return std::nullopt;
    auto pos2 = line.find(',', pos1);
    if (pos2 == std::string::npos)
        return std::nullopt;
    std::string number_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
    try {
        return std::stoi(number_str);
    } catch (...) {
        return std::nullopt;
    }
}

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    std::set<int> line_set;

    auto filtered_lines = f.ast | std::views::split('\n') |
                          std::views::transform([](const auto &&el) { return std::string(el.begin(), el.end()); }) |
                          std::views::filter([](const auto &str) {
                              return (extract_number(str).has_value() && !str.contains("comment"));
                          });

    std::ranges::for_each(filtered_lines, [&](const auto &str) { line_set.insert(extract_number(str).value()); });

    return line_set.size();
}
std::string CodeLinesCountMetric::Name() const { return "CodeLinesCountMetric"; }

}  // namespace analyser::metric::metric_impl
