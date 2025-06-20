#include "metric_impl/parameters_count.hpp"

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

    for (auto el : f.ast | std::views::split('\n')) {
        std::string substr(el.begin(), el.end());
        auto opt_val = extract_number(substr);
        if (opt_val.has_value()) {  // function definition placed on the 0-th line
            if (substr.contains("function_definition")) {
                func_definition_line = opt_val.value();
            } else if (substr.contains("identifier") && opt_val.value() == func_definition_line) {
                func_param_counter++;
            }
        }
    }

    func_param_counter--;  // to exclude "name: (identifier ...)" line

    return func_param_counter;
}

std::string CountParametersMetric::Name() const { return "CountParametersMetric"; }

}  // namespace analyser::metric::metric_impl
