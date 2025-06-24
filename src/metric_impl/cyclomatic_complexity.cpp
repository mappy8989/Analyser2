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
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    auto counter = std::ranges::count_if(f.ast | std::views::split(' '), [](auto &&elem) {
        std::string st(elem.begin(), elem.end());
        return (st.contains("if_statement") || st.contains("elif_clause") || st.contains("while_statement") ||
                st.contains("for_statement") || st.contains("try_statement") || st.contains("except_clause") ||
                st.contains("case_clause"));
    });

    counter++;  // initially base complexity of a function is 1

    return counter;
}
std::string CyclomaticComplexityMetric::Name() const { return "CyclomaticComplexityMetric"; }

}  // namespace analyser::metric::metric_impl
