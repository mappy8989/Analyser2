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
    double dbl_cnt = static_cast<double>(count);
    sum += metric_result.value;
    average = average * dbl_cnt / (dbl_cnt + 1) + metric_result.value / (dbl_cnt + 1);
    count++;
}

void SumAverageAccumulator::Reset() {
    count = 0;
    sum = 0;
    average = 0.0;
}
// здесь ваш код

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
