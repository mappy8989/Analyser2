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
    return files | std::views::transform([&](const auto &filename) {
               return analyser::function::FunctionExtractor().Get(filename);
           }) |
           rv::join | std::views::transform([&](const auto &func) {
               return std::make_pair(func, metric_extractor.Get(func));
           }) |
           rs::to<std::vector>();
}

auto SplitByClasses(
    const std::vector<std::pair<analyser::function::Function,
                                std::vector<analyser::metric::MetricResult>>> &analysis) {
    auto class_funcs =
        analysis |
        std::views::filter([](const auto &elem) { return elem.first.class_name.has_value(); }) |
        rs::to<std::vector>();

    rs::sort(class_funcs, [](const auto &a, const auto &b) {
        return a.first.class_name.value() < b.first.class_name.value();
    });

    return std::views::chunk_by(class_funcs,
                                [](const auto &a, const auto &b) {
                                    return a.first.class_name.value() == b.first.class_name.value();
                                }) |
           std::views::transform(
               [](auto &&chunk) { return std::vector(chunk.begin(), chunk.end()); }) |
           rs::to<std::vector>();
}

auto SplitByFiles(
    const std::vector<std::pair<analyser::function::Function,
                                std::vector<analyser::metric::MetricResult>>> &analysis) {
    std::vector<
        std::pair<analyser::function::Function, std::vector<analyser::metric::MetricResult>>>
        sorted_funcs(analysis.begin(), analysis.end());

    rs::sort(sorted_funcs,
             [](const auto &a, const auto &b) { return a.first.filename < b.first.filename; });

    return std::views::chunk_by(
               sorted_funcs,
               [](const auto &a, const auto &b) { return a.first.filename == b.first.filename; }) |
           std::views::transform(
               [](auto &&chunk) { return std::vector(chunk.begin(), chunk.end()); }) |
           rs::to<std::vector>();
}

void AccumulateFunctionAnalysis(
    const auto &analysis, const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    rs::for_each(analysis,
                 [&](const auto &elem) { accumulator.AccumulateNextFunctionResults(elem.second); });
}

}  // namespace analyser
