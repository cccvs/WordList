#include <stdexcept>
#include <gtest/gtest.h>

#include "test.h"
#include "../src/cli.h"

int argc;
char argv_raw[20][128];
char *argv[20];
char exc_msg[256];

void load_exc_case(const string &case_path) {
    FILE *file = fopen(case_path.data(), "r");
    fscanf_s(file, "%d", &argc);
    for (int i = 0; i < argc; ++i) {
        fscanf(file, "%s\n", argv_raw[i]);
        argv[i] = argv_raw[i];
    }
    fgets(exc_msg, sizeof(exc_msg), file);
    fclose(file);
}

void exc_test(const string &suite_name, const string &case_name) {
    chdir(suite_path(suite_name).data());
    load_exc_case(case_path(suite_name, case_name));
    string msg;
    try {
        throw logic_error(to_string(cli(argc, argv, false)));
    } catch (const logic_error &e) {
        ASSERT_STREQ(e.what(), exc_msg);
    }
    free_resource();
    remove("solution.txt");
}