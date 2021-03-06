#ifndef __Connection_H
#define __Connection_H

#include <boost/asio.hpp>

#include <string>
#include <thread>
#include <memory>

class ConnectionDelegate;

class Connection
: public std::enable_shared_from_this<Connection> {
public:
    static std::shared_ptr<Connection> Create(const std::string address, unsigned short port, std::shared_ptr<ConnectionDelegate> delegate);
    
    ~Connection();

    void write(const std::string &message);

private:
    Connection(std::shared_ptr<ConnectionDelegate> dlgt);

    void start(const std::string address, unsigned short port);
    void read();

    boost::asio::io_service service;
    boost::asio::io_service::work work;
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<ConnectionDelegate> delegate; 
    std::thread thread;

    char readBuffer[1024];
};

#endif  // __Connection_H
