#ifndef WORDLIST_CORE_H
#define WORDLIST_CORE_H

#include "vector"
#include "string"
#include "set"
#include "map"
#include "queue"
#include "algorithm"
#include "cassert"
#include "limits"

#define MAX_VERTEX 26
#define MAX_SCC MAX_VERTEX
#define MAX_EDGE 10010
#define MAX_CHAIN 20000

using namespace std;

typedef long long ll;

struct status {
    ll first, second;
    int cur_v;

    bool operator<(const status &o) const {
        return this->cur_v < o.cur_v ||
               (this->cur_v == o.cur_v && this->first < o.first) ||
               (this->cur_v == o.cur_v && this->first == o.first && this->second < o.second);
    }
};

extern "C" __declspec(dllexport) int gen_chains_all(char *words[], int len, char *result[], void *my_malloc(size_t));

extern "C" __declspec(dllexport) int gen_chain_word(char *words[], int len, char *result[], char head, char tail,
                                                    char reject, bool enable_loop, void *my_malloc(size_t));

extern "C" __declspec(dllexport) int gen_chain_char(char *words[], int len, char *result[], char head, char tail,
                                                    char reject, bool enable_loop, void *my_malloc(size_t));

#endif //WORDLIST_CORE_H