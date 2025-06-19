#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {

    auto counter = std::ranges::count_if(f.ast | std::views::split(' '), [](auto &&elem) {
        std::string st(elem.begin(), elem.end());
        return (st.contains("if") || st.contains("elif") || st.contains("while") || st.contains("for") ||
                st.contains("try") || st.contains("catch") || st.contains("finally") || st.contains("match") ||
                st.contains("case") || st.contains("assert"));
    });
}
std::string CyclomaticComplexityMetric::Name() const { return "CyclomaticComplexityMetric"; }

}  // namespace analyser::metric::metric_impl
