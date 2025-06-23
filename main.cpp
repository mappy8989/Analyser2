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

    std::println("!@");

    analyser::cmd::ProgramOptions options;
    // распарсите входные параметры

    analyser::file::File f("../files/sample.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CodeLinesCountMetric lines_counter;
    std::println("CyclomaticCCodeLinesCountMetricomplexityMetric");
    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclomatic_counter;
    analyser::metric::metric_impl::CountParametersMetric param_counter;

    analyser::metric::MetricExtractor extractor;
    // extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>(lines_counter));
    extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>(cyclomatic_counter));
    extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>(param_counter));

    std::vector<std::string> filenames = {"../files/sample.py", "../files/ifs.py", "../files/ex.py"};
    auto analysed_funs = analyser::AnalyseFunctions(filenames, extractor);
    std::ranges::for_each(analysed_funs, [](const auto &elem) {
        std::println("{}", elem.first.name);
        std::ranges::for_each(elem.second,
                              [](const auto &elem) { std::println("{} {}", elem.metric_name, elem.value); });
        std::println();
    });
    auto splitted = analyser::SplitByFiles(analysed_funs);

    analyser::metric_accumulator::MetricsAccumulator metric_acc;
    analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator av_acc;
    analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator cat_acc;
    analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator sum_av_acc;

    metric_acc.RegisterAccumulator(
        lines_counter.Name(),
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>(std::move(av_acc)));
    metric_acc.RegisterAccumulator(
        lines_counter.Name(),
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator>(
            std::move(cat_acc)));

    auto res2 = func_vec | std::views::transform([&](auto &&elem) { return lines_counter.Calculate(elem); }) |
                std::ranges::to<std::vector>();

    metric_acc.AccumulateNextFunctionResults(res2);
    auto res3 =
        metric_acc
            .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator>(
                lines_counter.Name());

    analyser::AccumulateFunctionAnalysis(splitted, metric_acc);

    // analyser::metric::MetricExtractor metric_extractor;
    // зарегистрируйте метрики в metric_extractor

    // запустите analyser::AnalyseFunctions
    // выведете результаты анализа на консоль

    // analyser::metric_accumulator::MetricsAccumulator accumulator;
    // зарегистрируйте аккумуляторы метрик в accumulator

    // запустите analyser::SplitByFiles
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества
    // результатов метрик выведете результаты на консоль

    // запустите analyser::SplitByClasses
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества
    // результатов метрик выведете результаты на консоль

    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
