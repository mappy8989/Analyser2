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
    for (auto &el : func_vec) {
        std::println("{}", lines_counter.Calculate(el).value);
    }

    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclomatic_counter;
    std::println("CyclomaticComplexityMetric");
    for (auto &el : func_vec) {
        std::println("{}", cyclomatic_counter.Calculate(el).value);
    }

    analyser::metric::metric_impl::CountParametersMetric param_counter;
    std::println("CountParametersMetric");
    for (auto &el : func_vec) {
        std::println("{}", param_counter.Calculate(el).value);
    }

    analyser::metric::MetricExtractor extractor;
    extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>(lines_counter));
    extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>(
            cyclomatic_counter));
    extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CountParametersMetric>(param_counter));

    std::vector<std::string> filenames = {"../files/sample.py", "../files/ifs.py",
                                          "../files/ex.py"};
    auto res = analyser::AnalyseFunctions(filenames, extractor);

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
