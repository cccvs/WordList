#include <iostream>
#include <set>

#include "utils.h"

using namespace std;

char *read_file(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = (char *)malloc(size + 1);
    fread(content, 1, size + 1, file);
    content[size] = '\0';
    fclose(file);
    return content;
}

void parse_words(char *content, char *words[], int &len) {
    bool flag = false;
    len = 0;
    for (int i = 0; content[i]; ++i) {
        if (isalpha(content[i])) {
            content[i] = (char)(content[i] | 32);
            if (flag) {
                words[len++] = content + i;
                flag = false;
            }
        } else {
            content[i] = '\0';
            flag = true;
        }
    }
}

void unique_words(char *words[], int &len) {
    set<string> unique;
    int i = 0;
    while (i < len) {
        if (unique.find(words[i]) != unique.end()) {
            words[i] = words[--len];
        } else {
            unique.emplace(words[i++]);
        }
    }
}

void write_result_to_file(char *result[], int len) {
    FILE *file = fopen("solution.txt", "w");
    for (int i = 0; i < len; ++i) {
        fputs(result[i], file);
        fputc('\n', file);
    }
    fclose(file);
}

void write_result_to_screen(char *result[], int len) {
    printf("%d\n", len);
    for (int i = 0; i < len; ++i) {
        puts(result[i]);
    }
}
