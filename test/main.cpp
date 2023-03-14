#include <gtest/gtest.h>

#include "test.h"

using namespace std;

#define TEST_FUNC(suite, case_) TEST(suite, case_) { test(#suite, #case_); }

TEST_FUNC(core_test, test_of_test)

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}