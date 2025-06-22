#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    std::vector<analyser::file::File> files_obj;
    std::vector<std::tuple<analyser::file::File, analyser::function::Function>> files_obj2;
    files_obj.reserve(files.size());

    for (const auto &filename : files) {
        files_obj.emplace_back(filename);
    }

    std::vector<analyser::function::Function> funcs;

    std::ranges::for_each(files_obj, [&](const auto &elem) {
        auto obj_funcs = analyser::function::FunctionExtractor().Get(elem);
        funcs.insert(funcs.end(), std::make_move_iterator(obj_funcs.begin()),
                     std::make_move_iterator(obj_funcs.end()));
    });
    std::vector<
        std::pair<analyser::function::Function, std::vector<analyser::metric::MetricResult>>>
        metrics;
    std::ranges::for_each(funcs, [&](const auto &elem) {
        metrics.emplace_back(std::make_pair(elem, metric_extractor.Get(elem)));
    });

    return metrics;
}

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(
    const auto &analysis, const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
