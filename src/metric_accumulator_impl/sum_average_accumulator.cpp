#include "metric_accumulator_impl/sum_average_accumulator.hpp"

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
void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += metric_result.value;
    count++;

    is_finalized = false;
}

void SumAverageAccumulator::Finalize() {
    average = static_cast<double>(sum) / count;
    is_finalized = true;
};

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("Result is not finalized!");
    }
    return SumAverage{sum, average};
};

void SumAverageAccumulator::Reset() {
    count = 0;
    sum = 0;
    average = 0.0;

    is_finalized = false;
}
// здесь ваш код

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
