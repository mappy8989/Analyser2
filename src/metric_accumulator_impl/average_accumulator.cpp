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
    double dbl_cnt = static_cast<double>(count);
    sum += metric_result.value;
    average = average * dbl_cnt / (dbl_cnt + 1) + (metric_result.value) / (dbl_cnt + 1);
    count++;
}

void AverageAccumulator::Reset() {
    count = 0;
    sum = 0;
    average = 0.0;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
