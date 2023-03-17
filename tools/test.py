import ctypes
import os
import sys
from random import randint, seed, choice
from subprocess import Popen, TimeoutExpired

from tools.checker import check
from tools.generator import ALPHA

ANS_USER = "htr"
ANS = -1
USERS = ["htr", "wxg"]
PROCESS = {}


def rand_argv(file_name, data_path):
    loop = file_name.endswith("r.txt")
    if not loop:
        yield ["-n", data_path]
    for mode in ["-w", "-c"]:
        argv = [mode, data_path]
        if randint(0, 1):
            argv += ["-h", choice(ALPHA)]
        if randint(0, 1):
            argv += ["-t", choice(ALPHA)]
        if randint(0, 1):
            argv += ["-j", choice(ALPHA)]
        if loop:
            argv += ["-r"]
        yield argv


def get_data_argv():
    for root, dirs, files in os.walk(sys.path[0] + "/data"):
        for f in files:
            for argv in rand_argv(f, os.path.join(root, f)):
                yield argv


def get_time(user):
    with PROCESS[user] as p:
        handle = p._handle
    creation_time = ctypes.c_ulonglong()
    exit_time = ctypes.c_ulonglong()
    kernel_time = ctypes.c_ulonglong()
    user_time = ctypes.c_ulonglong()
    ctypes.windll.kernel32.GetProcessTimes(
        handle,
        ctypes.byref(creation_time),
        ctypes.byref(exit_time),
        ctypes.byref(kernel_time),
        ctypes.byref(user_time),
    )
    real_time = (exit_time.value - creation_time.value) / 10000000
    cpu_time = (kernel_time.value + user_time.value) / 10000000
    return real_time, cpu_time


def run(user, argv):
    user_path = sys.path[0] + "/user/" + user
    with open(user_path + "/solution.txt", "w") as out:
        process = Popen("Wordlist.exe " + " ".join(argv), shell=True, cwd=user_path, stdout=out, stderr=out)
    return process


def output(user, argv):
    print("{0:5}".format(user + ":"), end="")
    try:
        PROCESS[user].communicate(timeout=300)
        with open(sys.path[0] + "/user/" + user + "/solution.txt") as out:
            result = out.read().strip().splitlines()
        res = check(argv, result)

        global ANS
        if user == ANS_USER:
            ANS = res
        elif res != ANS:
            raise RuntimeError("WA, " + str(res))

        print("AC, {2}, real_time is {0:.3f}, cpu_time is {1:.3f}".format(*get_time(user), res))
    except TimeoutExpired as _:
        PROCESS[user].kill()
        print("TLE: 300s")
    except RuntimeError as e:
        print(e)


if __name__ == '__main__':
    seed(0)
    for u in USERS:
        assert os.path.exists(sys.path[0] + "/user/" + u + "/Wordlist.exe")
    for a in get_data_argv():
        print("////////////", a[0], a[1][a[1].rfind("\\") + 1:], *a[2:], sep=" ")
        for u in USERS:
            PROCESS[u] = run(u, a)
        for u in USERS:
            output(u, a)
        print()
