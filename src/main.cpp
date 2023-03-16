#include <iostream>

#include "cli.h"

using namespace std;

int main(int argc, char *argv[]) {
    setbuf(stdout, nullptr);
    try {
        cli(argc - 1, argv + 1, true);
    } catch (const logic_error &e) {
        puts(e.what());
    }
    free_resource();
    return 0;
}