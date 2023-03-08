#include "utils.h"

using namespace std;

void write_results_to_file(char *result[], int len) {
    FILE *file = fopen("solution.txt", "w");
    for (int i = 0; i < len; ++i) {
        fputs(result[i], file);
        fputc('\n', file);
    }
    fclose(file);
}

void write_results_to_screen(char *result[], int len) {
    printf("%d\n", len);
    for (int i = 0; i < len; ++i) {
        puts(result[i]);
    }
}

int read_words(char *file_name, char *words[], int *len) {
    FILE *file;
    file = fopen(file_name, "r");
    if (file == nullptr) {
        printf("ERROR-1, file_path not found!\n");
    } else {
        // step 1, process .txt file_path to a set
        set<string> word_set;
        word_set.clear();
        int c = getc(file);
        while (c != EOF) {
            if (!isalpha(c)) {
                c = getc(file);
            } else {
                string s;
                while (isalpha(c)) {    // c is alpha here at first loop
                    s += (char)tolower(c);
                    c = getc(file);
                }
                word_set.insert(s);
            }
        }
        // step 2, process set<string> to char *words[]
        int cnt = 0;
        for (const auto &item: word_set) {
            char *t = (char *)malloc(item.size() + 1);
            for (int i = 0; i < item.size(); ++i) {
                *(t + i) = item[i];
            }
            *(t + item.size()) = '\0';
            words[cnt++] = t;
        }
        *len = cnt;
    }
    fclose(file);
    return 0;
}
