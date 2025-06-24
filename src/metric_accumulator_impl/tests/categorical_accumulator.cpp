

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(categorical_acc_test, simple_test) {
    analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator av_acc;
    av_acc.Accumulate({"test_metric_1", 1});
    av_acc.Accumulate({"test_metric_1", 2});
    av_acc.Accumulate({"test_metric_1", 3});

    av_acc.Accumulate({"test_metric_2", 10});
    av_acc.Accumulate({"test_metric_2", 20});
    av_acc.Accumulate({"test_metric_2", 30});

    ASSERT_NEAR(av_acc.Get().at("test_metric_1"), 2., 0.001);
    ASSERT_NEAR(av_acc.Get().at("test_metric_2"), 20., 0.001);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
