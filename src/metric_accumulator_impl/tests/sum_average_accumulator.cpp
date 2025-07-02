#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(sum_average_acc_test, simple_test) {
    analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator sum_av_acc;
    sum_av_acc.Accumulate({"test_metric", 10});
    sum_av_acc.Accumulate({"test_metric", 20});
    sum_av_acc.Accumulate({"test_metric", 30});

    sum_av_acc.Finalize();

    ASSERT_NEAR(sum_av_acc.Get().average, 20., 0.001);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
