#include <gtest/gtest.h>

#include "test.h"

using namespace std;

#define TEST_FUNC(suite, case_) TEST(suite, case_) { suite(#suite, #case_); }

TEST_FUNC(core_test, no_answer)
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

//TEST_FUNC(exc_test, a_two_self_ring)
//TEST_FUNC(exc_test, b_ring)
//TEST_FUNC(exc_test, c_result_too_long_1)
//TEST_FUNC(exc_test, c_result_too_long_2)
//TEST_FUNC(exc_test, d_flag_not_recognized)
//TEST_FUNC(exc_test, e_basic_flag_not_exist)
//TEST_FUNC(exc_test, f_basic_flag_conflict)
//TEST_FUNC(exc_test, g_flagN_conflict)
//TEST_FUNC(exc_test, h_file_name_not_exist)
//TEST_FUNC(exc_test, i_char_not_exist)
//TEST_FUNC(exc_test, j_char_not_recognized)
//TEST_FUNC(exc_test, k_arg_exceed)
//TEST_FUNC(exc_test, l_file_not_exist)
//TEST_FUNC(exc_test, m_file_cant_open)

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    system("pause");
    return r;
}