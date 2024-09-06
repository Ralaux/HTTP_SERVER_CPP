#include "utils.h"
#include <iostream>
#include <cstdlib>

void handle_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
