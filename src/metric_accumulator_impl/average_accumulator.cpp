#include "metric_accumulator_impl/average_accumulator.hpp"

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
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += metric_result.value;
    count++;

    is_finalized = false;
}

void AverageAccumulator::Finalize() {
    average = static_cast<double>(sum) / count;
    is_finalized = true;
};

double AverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("Result is not finalized!");
    }
    return average;
};

void AverageAccumulator::Reset() {
    count = 0;
    sum = 0;
    average = 0.0;
    is_finalized = false;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
