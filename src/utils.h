//
// Created by mona on 2023/3/7.
//

#ifndef UTILS_H
#define UTILS_H

#endif //UTILS_H

#include "iostream"
#include "set"
#include <cstdlib>

#define MAX_WORDS_LEN 10010
#define MAX_RESULTS_LEN 20010

using namespace std;

int read_words(char *file_name, char *words[], int *len);
void write_results_to_file(char *result[], int len);
void write_results_to_screen(char *result[], int len);