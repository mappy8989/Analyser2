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
    count++;
    sum += metric_result.value;
    average = average * count / (count + 1) + metric_result.value / (count + 1);
}

void AverageAccumulator::Reset() {
    count = 0;
    sum = 0;
    average = 0.0;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
