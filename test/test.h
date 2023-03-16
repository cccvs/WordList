#ifndef WORDLIST_TEST_H
#define WORDLIST_TEST_H

#include <string>

using namespace std;

string suite_path(const string &suite_name);

string case_path(const string &suite_name, const string &case_name);

void core_test(const string &suite_name, const string &case_name);

void exc_test(const string &suite_name, const string &case_name);

#endif //WORDLIST_TEST_H
