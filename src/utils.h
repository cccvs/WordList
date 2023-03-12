#ifndef UTILS_H
#define UTILS_H

#define MAX_WORDS_LEN 10010
#define MAX_RESULT_LEN 20010

void read_file(char *file_path, char *&content, int &size);

void parse_words(char *content, int size, char *words[], int &len);

void unique_words(char *words[], int &len);

void write_result_to_file(char *result[], int len);

void write_result_to_screen(char *result[], int len);

#endif //UTILS_H
