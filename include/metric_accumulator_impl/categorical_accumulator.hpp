#pragma once
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

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct CategoricalAccumulator : public IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override;

    void Finalize() override;

    void Reset() override;

    const std::unordered_map<std::string, double> &Get() const;

private:
    std::unordered_map<std::string, double> categories_freq;
    std::unordered_map<std::string, double> categories_data_counter;  // for correct average calculation
    bool is_finalized = false;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
