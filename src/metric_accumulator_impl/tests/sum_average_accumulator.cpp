#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(sum_average_acc_test, simple_test) {
    analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator av_acc;
    av_acc.Accumulate({"test_metric", 10});
    av_acc.Accumulate({"test_metric", 20});
    av_acc.Accumulate({"test_metric", 30});

    ASSERT_NEAR(av_acc.Get().average, 20., 0.001);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
