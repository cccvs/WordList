//
// Created by mona on 2023/3/7.
//

#ifndef WORDLIST_CORE_H
#define WORDLIST_CORE_H

#endif //WORDLIST_CORE_H
int gen_chain(char* words[], int len, char* result[]);
int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
int gen_chains_all(char* words[], int len, char* result[]);
int gen_chain_word_unique(char* words[], int len, char* result[]);
int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);