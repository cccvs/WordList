#ifndef WORDLIST_CORE_H
#define WORDLIST_CORE_H

int gen_chains_all(char *words[], int len, char *result[], void *malloc(size_t));

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop);

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop);

#endif //WORDLIST_CORE_H
