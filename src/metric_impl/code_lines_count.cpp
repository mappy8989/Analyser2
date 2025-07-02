#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <charconv>
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

std::optional<int> ExtractCodeLineNumber(const std::string_view line) {
    auto pos1 = line.find('[');
    if (pos1 == std::string::npos)
        return std::nullopt;
    auto pos2 = line.find(',', pos1);
    if (pos2 == std::string::npos)
        return std::nullopt;

    int result{};
    auto [ptr, ec] = std::from_chars(line.data() + pos1 + 1, line.data() + pos2 - pos1 - 1, result);
    if (ec == std::errc()) {
        return result;
    } else {
        throw std::runtime_error("Incorrect number");
    }
}

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    std::set<int> line_set;

    auto filtered_lines =
        f.ast | std::views::split('\n') | std::views::transform([](const auto &&el) {
            return std::string_view(el.begin(), el.end());
        }) |
        std::views::filter([](const auto &str) {
            return (ExtractCodeLineNumber(str).has_value() && !str.contains("comment"));
        });

    std::ranges::for_each(filtered_lines, [&](const auto &str) {
        line_set.insert(ExtractCodeLineNumber(str).value());
    });

    return line_set.size();
}
std::string CodeLinesCountMetric::Name() const { return "CodeLinesCountMetric"; }

}  // namespace analyser::metric::metric_impl
