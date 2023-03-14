#include <gtest/gtest.h>

#include "test.h"

using namespace std;

#define TEST_FUNC(suite, case_) TEST(suite, case_) { test(#suite, #case_); }

TEST_FUNC(core_test, test_of_test)
TEST_FUNC(core_test, test01)
TEST_FUNC(core_test, test02)
TEST_FUNC(core_test, test03)
TEST_FUNC(core_test, test04)
TEST_FUNC(core_test, test05)
TEST_FUNC(core_test, test06)
TEST_FUNC(core_test, test07)
TEST_FUNC(core_test, test08)
TEST_FUNC(core_test, test09)
TEST_FUNC(core_test, test10)
TEST_FUNC(core_test, test11)
TEST_FUNC(core_test, test12)
TEST_FUNC(core_test, test13)
TEST_FUNC(core_test, test14)
TEST_FUNC(core_test, test15)
TEST_FUNC(core_test, test16)
TEST_FUNC(core_test, test17)
TEST_FUNC(core_test, test18)
TEST_FUNC(core_test, test19)
TEST_FUNC(core_test, test20)
TEST_FUNC(core_test, test21)
TEST_FUNC(core_test, test22)
TEST_FUNC(core_test, test23)
TEST_FUNC(core_test, test24)
TEST_FUNC(core_test, test25)
TEST_FUNC(core_test, test26)
TEST_FUNC(core_test, test27)
TEST_FUNC(core_test, test28)
TEST_FUNC(core_test, test29)
TEST_FUNC(core_test, test30)


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}