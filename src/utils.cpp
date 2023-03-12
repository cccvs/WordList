#include <iostream>
#include <set>

#include "utils.h"

using namespace std;

void read_file(char *file_path, char *&content, int &size) {
    FILE *file = fopen(file_path, "rb");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    content = (char *)malloc(size + 1);
    fread(content, 1, size + 1, file);
    content[size] = '\0';
    fclose(file);
}

void parse_words(char *content, int size, char *words[], int &len) {
    bool flag = false;
    len = 0;
    for (int i = 0; i < size; ++i) {
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
        if (unique.find(words[i]) == unique.end()) {
            words[i] = words[len--];
        } else {
            unique.insert(words[i++]);
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

//int read_words(char *file_name, char *words[], int *len) {
//    FILE *file;
//    file = fopen(file_name, "r");
//    if (file == nullptr) {
//        printf("ERROR-1, file_path not found!\n");
//    } else {
//        // step 1, process .txt file_path to a set
//        set<string> word_set;
//        word_set.clear();
//        int c = getc(file);
//        while (c != EOF) {
//            if (!isalpha(c)) {
//                c = getc(file);
//            } else {
//                string s;
//                while (isalpha(c)) {    // c is alpha here at first loop
//                    s += (char)tolower(c);
//                    c = getc(file);
//                }
//                word_set.insert(s);
//            }
//        }
//        // step 2, process set<string> to char *words[]
//        int cnt = 0;
//        for (const auto &item: word_set) {
//            char *t = (char *)malloc(item.size() + 1);
//            for (int i = 0; i < item.size(); ++i) {
//                *(t + i) = item[i];
//            }
//            *(t + item.size()) = '\0';
//            words[cnt++] = t;
//        }
//        *len = cnt;
//    }
//    fclose(file);
//    return 0;
//}
