#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(cyclomatic_complexity_test, if_check) {
    analyser::file::File f("../../../src/metric_impl/tests/files/if.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclo_counter;

    ASSERT_EQ(cyclo_counter.Calculate(func_vec.at(0)).value, 2);
}

TEST(cyclomatic_complexity, match_check) {
    analyser::file::File f("../../../src/metric_impl/tests/files/match_case.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclo_counter;

    ASSERT_EQ(cyclo_counter.Calculate(func_vec.at(0)).value, 4);  // check only first function
}

TEST(cyclomatic_complexity, nested_if_check) {
    analyser::file::File f("../../../src/metric_impl/tests/files/nested_if.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CyclomaticComplexityMetric cyclo_counter;

    ASSERT_EQ(cyclo_counter.Calculate(func_vec.at(0)).value, 4);  // check only first function
}

}  // namespace analyser::metric::metric_impl
