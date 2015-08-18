#include "IrcClient.h"

#include <iostream>

int main(int, char**) {
    auto client = IrcClient::Create();
    client->connect("127.0.0.1", 6667);

    std::cin.get();
}
