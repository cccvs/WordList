#include <map>
#include <stdexcept>

#include "core.h"
#include "utils.h"

using namespace std;

enum Flag {
    N, W, C, H, T, J, R, NUM_OF_FLAG
};

bool flags[NUM_OF_FLAG];
char *file_path = nullptr;
char head;
char tail;
char reject;

char *words[MAX_WORDS_LEN + 5];
int len;
char *result[MAX_RESULT_LEN + 5];

map<char, Flag> helper = {
    {'n', N},
    {'w', W},
    {'c', C},
    {'h', H},
    {'t', T},
    {'j', J},
    {'r', R}
};

void parse_args(int argc, char *argv[]) {
    Flag last = NUM_OF_FLAG;
    for (int i = 1; i < argc; ++i) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            last = helper[arg[1]];
            flags[last] = true;
        } else {
            char c = (char)(arg[0] | 32);
            switch (last) {
                case N:
                case W:
                case C:
                    file_path = arg;
                    break;
                case H:
                    head = c;
                    break;
                case T:
                    tail = c;
                    break;
                case J:
                    reject = c;
                    break;
                default:
                    throw runtime_error("idk");
            }
        }
    }
}

int main(int argc, char *argv[]) {
    setbuf(stdout, nullptr);

    char *content = nullptr;
    int r;

    parse_args(argc, argv);
    // TODO check_argv

    content = read_file(file_path);
    parse_words(content, words, len);
    unique_words(words, len);

    if (flags[N]) {
        r = gen_chains_all(words, len, result, malloc);
    } else if (flags[W]) {
        r = gen_chain_word(words, len, result, head, tail, reject, flags[R], malloc);
    } else if (flags[C]) {
        r = gen_chain_char(words, len, result, head, tail, reject, flags[R], malloc);
    }

    if (flags[N]) {
        write_result_to_screen(result, r);
    } else {
        write_result_to_file(result, r);
    }

    free(content);
    free(*result);
    return 0;
}