import re


def get_words(data_path):
    with open(data_path, "r") as f:
        content = f.read()
    pattern = re.compile(r"[a-zA-Z]+")
    return [x.lower() for x in pattern.findall(content)]


def repeat_check(chain):
    unique = set()
    for word in chain:
        if word in unique:
            raise RuntimeError(f"WA, {word} repeat")
        unique.add(word)


def char_check(chain, head, tail, reject):
    if len(chain) == 0:
        return
    if head and chain[0][0] != head:
        raise RuntimeError(f"WA, {chain[0]}'s head is not {head}")
    if tail and chain[-1][-1] != tail:
        raise RuntimeError(f"WA, {chain[-1]}'s tail is not {tail}")
    if reject:
        for word in chain:
            if word[0] == reject:
                raise RuntimeError(f"WA, {word}'s head rejected")


def chain_check(chain):
    for i in range(1, len(chain)):
        if chain[i][0] != chain[i - 1][-1]:
            raise RuntimeError(f"WA, {chain[i - 1]} and {chain[i]} can't link")


def exist_check(chain, words):
    for word in chain:
        if word not in words:
            raise RuntimeError(f"WA, {word} not exist")


def check(argv, result):
    mode = argv[0][1]
    words = get_words(argv[1])
    head, tail, reject = "", "", ""
    i = 2
    while i < len(argv):
        if argv[i] == "-h":
            head = argv[i + 1]
        elif argv[i] == "-t":
            tail = argv[i + 1]
        elif argv[i] == "-j":
            reject = argv[i + 1]
        i = i + 2
    if mode == "n":
        res = result[0] if result else "Too many word chains!"
        repeat_check(result[1:])
        for chain in result[1:]:
            c = chain.strip().split(" ")
            repeat_check(c)
            chain_check(c)
            exist_check(c, words)
    else:
        if mode == "w":
            res = len(result)
        else:
            res = 0
            for word in result:
                res = res + len(word)
        char_check(result, head, tail, reject)
        repeat_check(result)
        chain_check(result)
        exist_check(result, words)
    return res
