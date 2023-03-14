#include <gtest/gtest.h>

#include "test.h"
#include "../src/core.h"

using namespace std;

char words_raw[50][30];
char *words[50];
set<string> words_query;
int len, ans;
char mode, head, tail, reject;
bool enable_cycle;

char *result[100];
int r;

void parse_case(const string &rel_path) {
    string f = __FILE__;
    string dir = f.substr(0, f.find_last_of('/') + 1);
    FILE *file = fopen((dir + rel_path).data(), "r");
    words_query.clear();
    fscanf_s(file, "%d", &len);
    for (int i = 0; i < len; ++i) {
        fscanf(file, "%s", words_raw[i]);
        words_query.emplace(words_raw[i]);
        words[i] = words_raw[i];
    }
    char argc[10];
    fscanf(file, "%s", argc);
    mode = argc[0];
    head = argc[1] == '-' ? '\0' : argc[1];
    tail = argc[2] == '-' ? '\0' : argc[2];
    reject = argc[3] == '-' ? '\0' : argc[3];
    enable_cycle = argc[4] == 'r';
    fscanf_s(file, "%d", &ans);
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
    ASSERT_TRUE(reject == '\0' || chain[0][0] != reject);
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

void test(const string &suite_name, const string &case_name) {
    parse_case("cases/" + suite_name + "/" + case_name + ".txt");
    memset(result, 0, sizeof(result));
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
    free(*result);
}
