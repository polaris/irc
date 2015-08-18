#include "IrcClient.h"
#include "Connection.h"

std::shared_ptr<IrcClient> IrcClient::Create() {
    return std::shared_ptr<IrcClient>(new IrcClient);
}

IrcClient::IrcClient() {
}

void IrcClient::connect(const std::string &address, unsigned short port) {
    connection = Connection::Create(address, port, shared_from_this());
} 

void IrcClient::onConnect() {
    connection->write("NICK guest");
    connection->write("USER guest 0 * :Ronnie Reagan");
}

void IrcClient::onError() {
}
