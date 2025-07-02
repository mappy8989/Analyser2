#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(parameters_count_test, if_check) {
    analyser::file::File f("../../../src/metric_impl/tests/files/comments.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CountParametersMetric param_counter;

    ASSERT_EQ(param_counter.Calculate(func_vec.at(0)).value, 3);
}

TEST(parameters_count_test, zero_params_check) {
    analyser::file::File f("../../../src/metric_impl/tests/files/exceptions.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CountParametersMetric param_counter;

    ASSERT_EQ(param_counter.Calculate(func_vec.at(0)).value, 0);
}

}  // namespace analyser::metric::metric_impl
