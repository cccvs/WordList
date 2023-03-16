#include <iostream>
#include <set>
#include <io.h>

#include "utils.h"

using namespace std;

char *read_file(char *file_path) {
    FILE *file;
    if (_access(file_path, 0)) {
        throw logic_error("file not exist: " + string(file_path));
    } else if ((file = fopen(file_path, "rb")) == nullptr) {
        throw logic_error("file can't open: " + string(file_path));
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = (char *)malloc(size + 1);
    if (content == nullptr) {
        throw logic_error("memory alloc failed: " + to_string(size / 1024 / 1024) + "MB");
    }
    fread(content, 1, size + 1, file);
    content[size] = '\0';
    fclose(file);
    return content;
}

void parse_words(char *content, char *words[], int &len) {
    bool flag = true;
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
