#ifndef __IrcClient_H
#define __IrcClient_H

#include "ConnectionDelegate.h"

#include <memory>
#include <string>

class Connection;

class IrcClient
: public ConnectionDelegate
, public std::enable_shared_from_this<IrcClient> {
public:
    static std::shared_ptr<IrcClient> Create(const char * const nickName, const char * const realName);

    void connect(const std::string &address, unsigned short port);

    void onConnect();

    void onError();

private:
    IrcClient(const char * const nickName, const char * const realName);

    std::shared_ptr<Connection> connection;
    std::string nickName;
    std::string realName;
};

#endif  // __IrcClient_H
