#include "test_main.hpp"
#include "gtest/gtest.h"
#include <climits>
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

struct dataValidationTestParams {
	int testNbr;
    std::vector<std::string> argv;
    char* err_msg;
};

class dataValidationTestSuite : public ::testing::TestWithParam<dataValidationTestParams> {};

TEST_P(dataValidationTestSuite, validParams) {
    dataValidationTestParams params = GetParam();

    char** argv = (char**)malloc(sizeof(char*) * (params.argv.size()+1));
    for (size_t i = 0; i< params.argv.size(); i++)
        argv[i] = (char*)params.argv[i].c_str();
    argv[params.argv.size()] = NULL;
    char* err = validate(argv);
    ASSERT_STREQ(params.err_msg, err);
    free(argv);
}

INSTANTIATE_TEST_SUITE_P(allCases, dataValidationTestSuite,
                         testing::Values(
                             dataValidationTestParams{0, {"10", "200", "100", "50"}, NULL},
                             dataValidationTestParams{1, {"-10", "200", "100", "50"}, (char*)"non-valid argument"},
                             dataValidationTestParams{2, {"-10"}, (char*)"non-valid argument"},
                             dataValidationTestParams{3, {}, (char*)"missing values"},
                             dataValidationTestParams{4, {"10", "-200", "100", "50"}, (char*)"non-valid argument"},
                             dataValidationTestParams{5, {"10", "200", "100", "50", "1", "0"}, (char*)"too many arguments"}
                         ));


struct extractNumberTestParams {
    std::string arg;
    unsigned long long want;
    int want_err_code;
};

class extractNumberTestSuite : public ::testing::TestWithParam<extractNumberTestParams> {};

TEST_P(extractNumberTestSuite, validParams) {
    extractNumberTestParams params = GetParam();
    unsigned long long got;
    int got_err_code = parse((char*)params.arg.c_str(), &got);
    ASSERT_EQ(params.want_err_code, got_err_code);
    ASSERT_EQ(params.want, got);
}

INSTANTIATE_TEST_SUITE_P(allCases, extractNumberTestSuite,
                         testing::Values(
                             extractNumberTestParams{"10", 10, 0},
                             extractNumberTestParams{"4294967296", 4294967296, 0},
                             extractNumberTestParams{"4", 4, 0},
                             extractNumberTestParams{"4294967295", UINT_MAX, 0},
                             extractNumberTestParams{"0", 0, 0},
                             extractNumberTestParams{"01", 1, 0},
                             extractNumberTestParams{"000000004294967295", UINT_MAX, 0},
                             extractNumberTestParams{"18446744073709551615", ULLONG_MAX, 0},
                             extractNumberTestParams{"18446744073709551616", 0, -1}
                         ));
