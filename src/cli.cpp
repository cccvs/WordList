#include <map>
#include <stdexcept>

#include "core.h"
#include "utils.h"

using namespace std;

enum flag {
    NONE, N, W, C, H, T, J, R, NUM_OF_FLAG
};

char *content;
char *words[MAX_WORDS_LEN + 5];
int len;
char *result[MAX_RESULT_LEN + 5];

map<char, flag> helper = {
    {'n', N},
    {'w', W},
    {'c', C},
    {'h', H},
    {'t', T},
    {'j', J},
    {'r', R}
};

void parse_args(int argc, char *argv[], bool flags[], char *values[]) {
    flag f = NONE;
    for (int i = 0; i < argc; ++i) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            f = helper[arg[1]];
            flags[f] = true;
            if (arg[1] == '\0' || arg[2] != '\0' || f == NONE) {
                throw logic_error("flag not recognized: " + string(arg));
            }
        } else {
            values[f] = argv[i];
            f = NONE;
        }
    }
}

void check_args(const bool flags[], char *values[]) {
    int basic_flag = flags[N] + flags[W] + flags[C];
    if (basic_flag == 0) {
        throw logic_error("basic flag not exist");
    } else if (basic_flag > 1) {
        string s;
        s += flags[N] ? " -n" : "";
        s += flags[W] ? " -w" : "";
        s += flags[C] ? " -c" : "";
        throw logic_error("basic flag conflict:" + s);
    } else if (flags[N] && (flags[H] || flags[T] || flags[J] || flags[R])) {
        string s = "-n";
        s += flags[H] ? " -h" : "";
        s += flags[T] ? " -t" : "";
        s += flags[J] ? " -j" : "";
        s += flags[R] ? " -r" : "";
        throw logic_error("flag-N conflict: " + s);
    }
    if (!(values[N] || values[W] || values[C])) {
        throw logic_error("file name not exist");
    }
    for (flag f = H; f <= J; f = flag(f + 1)) {
        if (flags[f]) {
            if (!values[f]) {
                throw logic_error("char not exist");
            } else if (values[f][1] != '\0' || !isalpha(values[f][0])) {
                throw logic_error("char not recognized: " + string(values[f]));
            }
        }
    }
    if (values[NONE] || values[R]) {
        throw logic_error("arg exceed: " + string(values[R] ? values[R] : values[NONE]));
    }
}

void free_resource() {
    free(content);
    free(result[0]);
}

int cli(int argc, char *argv[], bool output) {
    content = result[0] = nullptr;

    bool flags[NUM_OF_FLAG] = {};
    char *values[NUM_OF_FLAG] = {};
    parse_args(argc, argv, flags, values);
    check_args(flags, values);

    char *file_path = flags[N] ? values[N] : flags[W] ? values[W] : values[C];
    char head = flags[H] ? values[H][0] : '\0';
    char tail = flags[T] ? values[T][0] : '\0';
    char reject = flags[J] ? values[J][0] : '\0';
    bool enable_loop = flags[R];
    int r;
    content = read_file(file_path);
    parse_words(content, words, len);
    unique_words(words, len);

    if (flags[N]) {
        r = gen_chains_all(words, len, result, malloc);
    } else if (flags[W]) {
        r = gen_chain_word(words, len, result, head, tail, reject, enable_loop, malloc);
    } else {
        r = gen_chain_char(words, len, result, head, tail, reject, enable_loop, malloc);
    }

    if (output) {
        if (flags[N]) {
            write_result_to_screen(result, r);
        } else {
            write_result_to_file(result, r);
        }
    }

    return r;
}
