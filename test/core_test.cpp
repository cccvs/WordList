#include <gtest/gtest.h>

#include "test.h"
#include "../src/core.h"

using namespace std;

static char words_raw[50][30];
static char *words[50];
static set<string> words_query;
static int len, ans;
static char mode, head, tail, reject;
static bool enable_cycle;

static char *result[100];
static int r;

string suite_path(const string &suite_name) {
    string f = __FILE__;
    string dir = f.substr(0, f.find_last_of('/'));
    string path = dir + "/cases/" + suite_name + "/";
    return path;
}

string case_path(const string &suite_name, const string &case_name) {
    return suite_path(suite_name) + case_name + ".txt";
}

void load_core_case(const string &case_path) {
    FILE *file = fopen(case_path.data(), "r");
    words_query.clear();
    fscanf_s(file, "%d", &len);
    for (int i = 0; i < len; ++i) {
        fscanf(file, "%s", words_raw[i]);
        words_query.emplace(words_raw[i]);
        words[i] = words_raw[i];
    }
    char args[10];
    fscanf(file, "%s", args);
    mode = args[0];
    head = args[1] == '-' ? '\0' : args[1];
    tail = args[2] == '-' ? '\0' : args[2];
    reject = args[3] == '-' ? '\0' : args[3];
    enable_cycle = args[4] == 'r';
    fscanf_s(file, "%d", &ans);
    fclose(file);
}

void split_result(char *res_split[][100], int res_split_len[]) {
    bool flag;
    for (int i = 0; i < r; ++i) {
        res_split_len[i] = 0;
        flag = true;
        for (int j = 0; result[i][j]; ++j) {
            if (result[i][j] != ' ') {
                if (flag) {
                    res_split[i][res_split_len[i]++] = result[i] + j;
                    flag = false;
                }
            } else {
                result[i][j] = '\0';
                flag = true;
            }
        }
    }
}

void repeat_check(char *chain[], int l) {
    set<string> s;
    for (int i = 0; i < l; ++i) {
        ASSERT_TRUE(s.find(chain[i]) == s.end());
        s.emplace(chain[i]);
    }
}

void char_check(char *chain[], int l) {
    ASSERT_TRUE(head == '\0' || chain[0][0] == head);
    ASSERT_TRUE(tail == '\0' || chain[l - 1][strlen(chain[l - 1]) - 1] == tail);
    for (int i = 0; i < l; ++i) {
        ASSERT_TRUE(reject == '\0' || chain[i][0] != reject);
    }
}

void chain_check(char *chain[], int l) {
    for (int i = 1; i < l; ++i) {
        ASSERT_TRUE(chain[i][0] == chain[i - 1][strlen(chain[i - 1]) - 1]);
    }
}

void exist_check(char *chain[], int l) {
    for (int i = 0; i < l; ++i) {
        ASSERT_TRUE(words_query.find(chain[i]) != words_query.end());
    }
}

void check() {
    ASSERT_EQ(r, ans);
    if (result[0] == nullptr) {
        return;
    }
    if (mode == 'n') {
        char *split[100][100];
        int split_len[100];
        repeat_check(result, r);
        split_result(split, split_len);
        for (int i = 0; i < r; ++i) {
            repeat_check(split[i], split_len[i]);
            chain_check(split[i], split_len[i]);
            exist_check(split[i], split_len[i]);
        }
    } else {
        char_check(result, r);
        repeat_check(result, r);
        chain_check(result, r);
        exist_check(result, r);
    }
}

void core_test(const string &suite_name, const string &case_name) {
    load_core_case(case_path(suite_name, case_name));
    result[0] = nullptr;
    if (mode == 'n') {
        r = gen_chains_all(words, len, result, malloc);
    } else if (mode == 'w') {
        r = gen_chain_word(words, len, result, head, tail, reject, enable_cycle, malloc);
    } else if (mode == 'c') {
        r = gen_chain_char(words, len, result, head, tail, reject, enable_cycle, malloc);
    } else {
        throw runtime_error("mode error");
    }
    check();
    free(result[0]);
}
