#include <stdio.h>
#include "server.hpp"
#include "packet.hpp"
#include "match.hpp"



int main() {

    Server s;

    s.serverThread.join();
    return 0;
}