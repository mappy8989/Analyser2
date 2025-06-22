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
        try {
            accumulators.at(elem.metric_name)->Accumulate(elem);
        } catch (...) {
            throw std::runtime_error("No aggregated accumulator found for " + elem.metric_name);
        }
    });
}
template <typename Accumulator>
void MetricsAccumulator::RegisterAccumulator(const std::string &metric_name,
                                             std::unique_ptr<Accumulator> acc) {
    accumulators.insert_or_assign(metric_name, std::move(acc));
}

void MetricsAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators, [&](const auto &elem) { elem.second->Reset(); });
}

}  // namespace analyser::metric_accumulator
