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

auto SplitByClasses(
    const std::vector<std::pair<analyser::function::Function,
                                std::vector<analyser::metric::MetricResult>>> &analysis) {
    auto class_funcs =
        analysis |
        std::views::filter([](const auto &elem) { return elem.first.class_name.has_value(); }) |
        std::ranges::to<std::vector>();

    std::ranges::sort(class_funcs, [](const auto &a, const auto &b) {
        return a.first.class_name.value() < b.first.class_name.value();
    });

    return std::views::chunk_by(class_funcs, [](const auto &a, const auto &b) {
        return a.first.class_name.value() == b.first.class_name.value();
    });
}

auto SplitByFiles(
    const std::vector<std::pair<analyser::function::Function,
                                std::vector<analyser::metric::MetricResult>>> &analysis) {
    std::vector<
        std::pair<analyser::function::Function, std::vector<analyser::metric::MetricResult>>>
        sorted_funcs(analysis.begin(), analysis.end());

    std::ranges::sort(sorted_funcs, [](const auto &a, const auto &b) {
        return a.first.filename < b.first.filename;
    });

    return std::views::chunk_by(sorted_funcs, [](const auto &a, const auto &b) {
        return a.first.filename == b.first.filename;
    });
}

void AccumulateFunctionAnalysis(
    const auto &analysis, const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
