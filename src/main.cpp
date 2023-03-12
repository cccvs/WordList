#include <map>
#include <stdexcept>

#include "core.h"
#include "utils.h"

using namespace std;

enum Flag {
    MAX_VERTEX, W, C, H, T, J, R, NUM_OF_FLAG
};

bool flags[NUM_OF_FLAG];
char *file_path = nullptr;
char head;
char tail;
char jail;

char *words[MAX_WORDS_LEN];
int len;
char *result[MAX_RESULT_LEN];

map<char, Flag> helper = {
    {'n', MAX_VERTEX},
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
                case MAX_VERTEX:
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
                    jail = c;
                    break;
                default:
                    throw runtime_error("idk");
            }
        }
    }
}

int dispatch() {
    int r = 0;
    if (flags[MAX_VERTEX]) {
        r = gen_chains_all(words, len, result); // TODO -j ?
    } else if (flags[W]) {
        r = gen_chain_word(words, len, result, head, tail, jail, flags[R]);
    } else if (flags[C]) {
        r = gen_chain_char(words, len, result, head, tail, jail, flags[R]);
    }
    return r;
}

int main(int argc, char *argv[]) {
    setbuf(stdout, nullptr); // TODO ?

    char *context = nullptr;
    int size = 0;
    int r;

    parse_args(argc, argv);
    // TODO check_argv

    read_file(file_path, context, size);
    parse_words(context, size, words, len);
    unique_words(words, len);

    if ((r = dispatch()) < 0) {
        printf("error %d\n", r);
    } else if (flags[MAX_VERTEX]) {
        write_results_to_screen(result, r);
    } else {
        write_result_to_file(result, r);
    }
    return 0;
}