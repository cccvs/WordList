#include "main.h"

int main() {
    test();
    return 0;
}

int get_input(char *file_name, char *words[], int *len) {
    FILE *file;
    file = fopen(file_name, "r");
    if (file == nullptr) {
        printf("ERROR-1, file not found!\n");
    } else {
        // step 1, process .txt file to a set
        set<string> word_set;
        word_set.clear();
        int c = getc(file);
        while (c != EOF) {
            if (!isalpha(c)) {
                c = getc(file);
            } else {
                string s;
                while (isalpha(c)) {    // c is alpha here at first loop
                    s += (char) tolower(c);
                    c = getc(file);
                }
                word_set.insert(s);
            }
        }
        // step 2, process set<string> to char *words[]
        int cnt = 0;
        for (const auto &item: word_set) {
            char *t = (char *) malloc(item.size() + 1);
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

void test() {
    char file_name[] = "src/input.txt";
    char *words[MAX_LEN];
    int len;
    get_input(file_name, words, &len);
    for (int i = 0; i < len; ++i) {
        cout << words[i] << ' ';
    }
    cout << endl << len;
}