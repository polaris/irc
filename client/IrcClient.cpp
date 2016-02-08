#include "IrcClient.h"
#include "Connection.h"

#include <boost/format.hpp>

#include <iostream>

std::shared_ptr<IrcClient> IrcClient::Create(const char * const nickName, const char * const realName) {
    return std::shared_ptr<IrcClient>(new IrcClient(nickName, realName));
}

IrcClient::IrcClient(const char * const nn, const char * const rn)
: nickName(nn)
, realName(rn) {
}

void IrcClient::connect(const std::string &address, unsigned short port) {
    connection = Connection::Create(address, port, shared_from_this());
} 

void IrcClient::onConnect() {
    connection->write(createNickMessage(nickName));
    connection->write(createUserMessage(nickName, realName, false, true));
}

void IrcClient::onMessage(const std::string &message) {
    std::cout << message;
    if (message.compare(0, 4, "PING") == 0) {
        connection->write("PONG :irc.example.net");
    }
}

void IrcClient::onError() {
}

std::string IrcClient::createNickMessage(const std::string &nickName) {
    return boost::str(boost::format("NICK %1%") % nickName);
}

std::string IrcClient::createUserMessage(const std::string &nickName, const std::string &realName, bool receiveWallops, bool invisible) {
    unsigned short mode = 0;
    mode = receiveWallops ? mode | 4 : mode;
    mode = invisible ? mode | 8 : mode;
    return boost::str(boost::format("USER %1% %2% * :%3%") % nickName % mode % realName);
}
