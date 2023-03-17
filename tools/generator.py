import json
import os
import sys
from random import randint, shuffle, choice

ALPHA = [chr(ord("a") + i) for i in range(26)]
DELIMITER = " \t\n!/09:@[`{~"


def rand_word(steep, head, tail):
    word_len = choice([1, 2, 4, 8, 16, 32]) if steep else randint(1, 20)
    if head != tail and word_len == 1:
        word_len = 2
    if head == tail and word_len == 1:
        return head
    else:
        return head + "".join([choice(ALPHA) for _ in range(word_len - 2)]) + tail


def rand_mix():
    return "".join([choice(DELIMITER) for _ in range(randint(1, 3))])


def gen_graph(num, loop):
    graph = [[0 for _ in range(26)] for _ in range(26)]
    for i in range(num):
        while True:
            head = randint(0, 25)
            tail = randint(0 if loop else head, 25)
            if head != tail or loop or graph[head][tail] == 0:
                break
        graph[head][tail] = graph[head][tail] + 1
    return graph


def gen_words(graph, steep, repeat):
    seq = ALPHA.copy()
    shuffle(seq)
    words = []
    for i in range(26):
        for j in range(26):
            head, tail = seq[i], seq[j]
            if repeat:
                words += [rand_word(steep, head, tail) for _ in range(graph[i][j])]
            else:
                words_unique = set()
                while len(words_unique) < graph[i][j]:
                    words_unique.add(rand_word(steep, head, tail))
                words += words_unique
    return words


def generate(num, steep, repeat, mix, loop):
    graph = gen_graph(num, loop)
    words = gen_words(graph, steep, repeat)
    if not mix:
        words.sort()
        s = "\n".join(words)
    else:
        s = rand_mix()
        for i in range(len(words)):
            s += "".join(map(lambda c: c if randint(0, 1) else c.upper(), words[i]))
            s += rand_mix()
    return s


if __name__ == '__main__':
    with open(sys.path[0] + "/config.json", "r") as f:
        config = json.load(f)
    data_path = sys.path[0] + "/data/"
    if not os.path.exists(data_path):
        os.mkdir(data_path)
    for name, mode in config.items():
        if not os.path.exists(data_path + name):
            os.mkdir(data_path + name)
        os.chdir(data_path + name)
        assert len(mode["word_num"]) == len(mode["case_num"])
        for i in range(len(mode["word_num"])):
            for j in range(mode["case_num"][i]):
                n = mode["word_num"][i]
                st = mode["steep"] if "steep" in mode else randint(0, 1)
                re = mode["repeat"] if "repeat" in mode else randint(0, 1)
                mi = mode["mix"] if "mix" in mode else randint(0, 1)
                lo = mode["loop"] if "loop" in mode else randint(0, 1)
                file_name = f"{name}_{n}_{j}{'_r' if lo else ''}.txt"
                with open(file_name, "w") as f:
                    f.write(generate(n, st, re, mi, lo))
