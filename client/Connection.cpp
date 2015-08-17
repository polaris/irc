#include "Connection.h"
#include "ConnectionDelegate.h"

#include <iostream>

std::shared_ptr<Connection> Connection::Create(const std::string address, unsigned short port, std::shared_ptr<ConnectionDelegate> delegate) {
    auto connection = std::shared_ptr<Connection>(new Connection(delegate));
    connection->start(address, port);
    return connection;
}

Connection::Connection(std::shared_ptr<ConnectionDelegate> dlgt)
: service()
, work(service)
, socket(service)
, delegate(dlgt)
, thread(boost::bind(&io_service::run, &service)) {
}

Connection::~Connection() {
    service.stop();
    thread.join();
}

void Connection::start(const std::string address, unsigned short port) {
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);
    socket.async_connect(endpoint, [this] (const boost::system::error_code &err) {
        if (!err) {
            read();
            delegate->onConnect();
        } else {
            std::cerr << "Failed to connect to server. Error: " << err.message() << std::endl;
        }
    });
}

void Connection::write(const std::string &message) {
    socket.async_send(buffer(message + "\r\n"),
        boost::bind(&Connection::onWrite, shared_from_this(), _1, _2));
}

void Connection::onWrite(const boost::system::error_code &err, size_t bytes) {
    if (!err) {
        std::cout << bytes << " written" << std::endl;
    } else {
        std::cerr << "Failed to write to server. Error: " << err.message() << std::endl;
    }
}

void Connection::read() {
    socket.async_receive(buffer(readBuffer), 
        boost::bind(&Connection::onRead, shared_from_this(), _1, _2));        
}

void Connection::onRead(const boost::system::error_code &err, size_t bytes) {
    if (!err) {
        std::string copy(readBuffer, bytes);
        std::cout << copy;
        read();
    } else {
        if (err == boost::asio::error::eof) {
            std::cerr << "Connection closed" << std::endl;
        } else {
            std::cerr << "Failed to read. Error: " << err.message() << std::endl;
        }
    }
}
