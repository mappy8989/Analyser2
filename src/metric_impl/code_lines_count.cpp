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

    for (auto el : f.ast | std::views::split('\n')) {
        std::string substr(el.begin(), el.end());
        auto opt_val = extract_number(substr);
        if (opt_val.has_value() && !substr.contains("comment")) {
            line_set.insert(opt_val.value());
        }
    }
    return line_set.size();
}
std::string CodeLinesCountMetric::Name() const { return "CodeLinesCountMetric"; }

}  // namespace analyser::metric::metric_impl
