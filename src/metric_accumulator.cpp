#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(
    const std::vector<metric::MetricResult> &metric_results) const {

    std::ranges::for_each(metric_results, [&](const auto &elem) {
        if (auto it = accumulators.find(elem.metric_name); it != accumulators.end()) [[likely]]
            it->second->Accumulate(elem);
        else {
            throw std::runtime_error("No aggregated accumulator found for " + elem.metric_name);
        }
    });
}

void MetricsAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators, [&](const auto &elem) { elem.second->Reset(); });
}

}  // namespace analyser::metric_accumulator
