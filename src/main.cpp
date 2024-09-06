#include "server.h"

int main() {
    const char* host = "127.0.0.1";
    int port = 8080;
    start_server(host, port);
    return 0;
}