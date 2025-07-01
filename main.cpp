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

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

namespace MI = analyser::metric::metric_impl;
namespace MAI = analyser::metric_accumulator::metric_accumulator_impl;

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (options.Parse(argc, argv) == false) {
        // just exit without error
        return 0;
    }

    const std::vector<std::string> filenames = options.GetFiles();

    MI::CodeLinesCountMetric lines_counter;
    MI::CyclomaticComplexityMetric cyclomatic_counter;
    MI::CountParametersMetric param_counter;

    analyser::metric::MetricExtractor extractor;
    extractor.RegisterMetric(std::make_unique<MI::CodeLinesCountMetric>(lines_counter));
    extractor.RegisterMetric(std::make_unique<MI::CyclomaticComplexityMetric>(cyclomatic_counter));
    extractor.RegisterMetric(std::make_unique<MI::CountParametersMetric>(param_counter));

    auto analysed_funcs = analyser::AnalyseFunctions(filenames, extractor);
    std::ranges::for_each(analysed_funcs, [](const auto &func) {
        if (func.first.class_name.has_value()) {
            std::println("{}[::{}]::{}", func.first.filename, func.first.class_name.value(),
                         func.first.name);
        } else {
            std::println("{}::{}", func.first.filename, func.first.name);
        };

        std::ranges::for_each(func.second, [](const auto &metric) {
            std::println("{}: {}", metric.metric_name, metric.value);
        });
        std::println();
    });

    analyser::metric_accumulator::MetricsAccumulator metric_acc;

    metric_acc.RegisterAccumulator(lines_counter.Name(),
                                   std::make_unique<MAI::AverageAccumulator>());
    metric_acc.RegisterAccumulator(param_counter.Name(),
                                   std::make_unique<MAI::CategoricalAccumulator>());
    metric_acc.RegisterAccumulator(cyclomatic_counter.Name(),
                                   std::make_unique<MAI::SumAverageAccumulator>());

    auto splitted_by_files = analyser::SplitByFiles(analysed_funcs);
    std::println();
    std::ranges::for_each(splitted_by_files, [&](auto &chunk) {
        std::println("Accumulated Analysis for file {}:", chunk.front().first.filename);
        analyser::AccumulateFunctionAnalysis(chunk, metric_acc);
        auto acc_res =
            metric_acc.GetFinalizedAccumulator<MAI::AverageAccumulator>(lines_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        auto sum_acc_res = metric_acc.GetFinalizedAccumulator<MAI::SumAverageAccumulator>(
            cyclomatic_counter.Name());
        std::println("{}: {}", lines_counter.Name(), sum_acc_res.Get().average);
        auto cat_acc_res =
            metric_acc.GetFinalizedAccumulator<MAI::CategoricalAccumulator>(param_counter.Name());
        std::ranges::for_each(cat_acc_res.Get(), [&](const auto &elem) {
            std::println("{}: {} {}", lines_counter.Name(), elem.first, elem.second);
        });
        std::println();
    });

    auto splitted_by_classes = analyser::SplitByClasses(analysed_funcs);
    std::println();
    std::ranges::for_each(splitted_by_classes, [&](auto &chunk) {
        std::println("Accumulated Analysis for class {}:", chunk.front().first.class_name.value());
        analyser::AccumulateFunctionAnalysis(chunk, metric_acc);
        auto acc_res =
            metric_acc.GetFinalizedAccumulator<MAI::AverageAccumulator>(lines_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        auto sum_acc_res = metric_acc.GetFinalizedAccumulator<MAI::SumAverageAccumulator>(
            cyclomatic_counter.Name());
        std::println("{}: {}", lines_counter.Name(), sum_acc_res.Get().average);
        auto cat_acc_res =
            metric_acc.GetFinalizedAccumulator<MAI::CategoricalAccumulator>(param_counter.Name());
        std::ranges::for_each(cat_acc_res.Get(), [&](const auto &elem) {
            std::println("{}: {} {}", lines_counter.Name(), elem.first, elem.second);
        });
        std::println();
    });

    return 0;
}  // namespace MI=MIint main(intargc,char*))
