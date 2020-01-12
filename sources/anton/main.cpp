#include "http_server.hpp"

int main() {
    http_server(80).start();
    return 0;
}