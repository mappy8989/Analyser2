#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(code_lines_count_test, comments_check) {
    analyser::file::File f("/workspaces/Analyser2/src/metric_impl/tests/files/comments.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CodeLinesCountMetric lines_counter;

    ASSERT_EQ(lines_counter.Calculate(func_vec.at(0)).value, 4);
}

TEST(code_lines_count_test, gaps_check) {
    analyser::file::File f("/workspaces/Analyser2/src/metric_impl/tests/files/many_lines.py");
    analyser::function::FunctionExtractor f_extr;
    std::vector func_vec = f_extr.Get(f);

    analyser::metric::metric_impl::CodeLinesCountMetric lines_counter;

    ASSERT_EQ(lines_counter.Calculate(func_vec.at(0)).value, 11);  // check only first function
}

}  // namespace analyser::metric::metric_impl
