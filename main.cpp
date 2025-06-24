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

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (options.Parse(argc, argv) == false) {
        // just exit without error
        return 0;
    }

    const std::vector<std::string> filenames = options.GetFiles();

    analyser::metric::metric_impl::CodeLinesCountMetric lines_counter;
    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclomatic_counter;
    analyser::metric::metric_impl::CountParametersMetric param_counter;

    analyser::metric::MetricExtractor extractor;
    extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>(lines_counter));
    extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>(cyclomatic_counter));
    extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>(param_counter));

    auto analysed_funcs = analyser::AnalyseFunctions(filenames, extractor);
    std::ranges::for_each(analysed_funcs, [](const auto &func) {
        if (func.first.class_name.has_value()) {
            std::println("{}[::{}]::{}", func.first.filename, func.first.class_name.value(), func.first.name);
        } else {
            std::println("{}::{}", func.first.filename, func.first.name);
        };

        std::ranges::for_each(func.second,
                              [](const auto &metric) { std::println("{}: {}", metric.metric_name, metric.value); });
        std::println();
    });

    analyser::metric_accumulator::MetricsAccumulator metric_acc;
    analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator av_acc;
    analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator cat_acc;
    analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator sum_av_acc;

    metric_acc.RegisterAccumulator(
        lines_counter.Name(),
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(av_acc));
    metric_acc.RegisterAccumulator(
        param_counter.Name(),
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(av_acc));
    metric_acc.RegisterAccumulator(
        cyclomatic_counter.Name(),
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(av_acc));

    auto splitted_by_files = analyser::SplitByFiles(analysed_funcs);
    std::println();
    std::ranges::for_each(splitted_by_files, [&](auto &chunk) {
        std::println("Accumulated Analysis for file {}:", chunk.front().first.filename);
        analyser::AccumulateFunctionAnalysis(chunk, metric_acc);
        auto acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    lines_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    cyclomatic_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    param_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        std::println();
    });

    auto splitted_by_classes = analyser::SplitByClasses(analysed_funcs);
    std::println();
    std::ranges::for_each(splitted_by_classes, [&](auto &chunk) {
        std::println("Accumulated Analysis for class {}:", chunk.front().first.class_name.value());
        analyser::AccumulateFunctionAnalysis(chunk, metric_acc);
        auto acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    lines_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    cyclomatic_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        acc_res =
            metric_acc
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(
                    param_counter.Name());
        std::println("{}: {}", lines_counter.Name(), acc_res.Get());
        std::println();
    });

    return 0;
}
