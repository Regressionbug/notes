#include "tcp.h"

int main() {
    TCP::Server server("127.0.0.1", 1989);
    server.start();
}
