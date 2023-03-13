#ifndef UTILS_H
#define UTILS_H

#define MAX_WORDS_LEN 10000
#define MAX_RESULT_LEN 20000

char *read_file(char *file_path);

void parse_words(char *content, char *words[], int &len);

void unique_words(char *words[], int &len);

void write_result_to_file(char *result[], int len);

void write_result_to_screen(char *result[], int len);

#endif //UTILS_H
