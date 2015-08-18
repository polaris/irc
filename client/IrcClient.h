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
    static std::shared_ptr<IrcClient> Create();

    void connect(const std::string &address, unsigned short port);

    void onConnect();

    void onError();

private:
    IrcClient();

    std::shared_ptr<Connection> connection;
};

#endif  // __IrcClient_H
