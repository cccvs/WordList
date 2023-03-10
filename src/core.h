//
// Created by mona on 2023/3/7.
//

#ifndef WORDLIST_CORE_H
#define WORDLIST_CORE_H

#endif //WORDLIST_CORE_H

#include "vector"
#include "string"
#include "set"
#include "queue"
#include "cassert"
#include "limits"

#define MAX_VERTEX 26
#define MAX_SCC MAX_VERTEX
#define MAX_EDGE 10010
#define MAX_CHAIN 20000

using namespace std;

typedef long long ll;
typedef pair<ll, ll> ll_pair;

int gen_chains_all(char *words[], int len, char *result[], void *malloc(size_t));
int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop);
int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop);

