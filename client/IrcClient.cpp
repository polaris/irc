#include "IrcClient.h"
#include "Connection.h"

#include <boost/format.hpp>

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
    connection->write(boost::str(boost::format("NICK %1%") % nickName));
    connection->write(boost::str(boost::format("USER %1% 0 * :%2%") % nickName % realName));
}

void IrcClient::onError() {
}
