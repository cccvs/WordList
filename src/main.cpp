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
char h_char = '\0';
char t_char = '\0';
char j_char = '\0';

char *words[MAX_WORDS_LEN];
int len;
char *result[MAX_RESULTS_LEN];

map<char, Flag> helper = {
    {'n', MAX_VERTEX},
    {'w', W},
    {'c', C},
    {'h', H},
    {'t', T},
    {'j', J},
    {'r', R}
};

void parse(int argc, char *argv[]) {
    Flag last = NUM_OF_FLAG;
    for (int i = 0; i < argc; ++i) {
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
                    h_char = c;
                    break;
                case T:
                    t_char = c;
                    break;
                case J:
                    j_char = c;
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
        r = gen_chain_word(words, len, result, h_char, t_char, flags[R]);
    } else if (flags[C]) {
        r = gen_chain_char(words, len, result, h_char, t_char, flags[R]);
    }
    return r;
}

int main(int argc, char *argv[]) {
    int r;

    parse(argc, argv);
    // TODO check_argv
    read_words(file_path, words, &len);
    r = dispatch();

    if (r != 0) {
        printf("error %d\n", r);
    } else if (flags[MAX_VERTEX]) {
        write_results_to_screen(result, r);
    } else {
        write_results_to_file(result, r);
    }
    return 0;
}