#include <iostream>
#include <memory>
#include <string>

#include "Connection.h"
#include "ConnectionDelegate.h"

class IrcClient
: public ConnectionDelegate
, public std::enable_shared_from_this<IrcClient> {
public:
    static std::shared_ptr<IrcClient> Create() {
        return std::shared_ptr<IrcClient>(new IrcClient);
    }

    void connect(const std::string &address, unsigned short port) {
        connection = Connection::Create(address, port, shared_from_this());
    } 

    void onConnect() {
        connection->write("NICK guest");
        connection->write("USER guest 0 * :Ronnie Reagan");
    }

    void onError() {
    }

private:
    IrcClient() {
    }

    std::shared_ptr<Connection> connection;
};

int main(int, char**) {
    auto client = IrcClient::Create();
    client->connect("127.0.0.1", 6667);

    std::cin.get();
}
