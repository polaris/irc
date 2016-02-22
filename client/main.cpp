#include "IrcClient.h"
#include "IrcMessage.h"

#include <string>
#include <iostream>

int main() {
    typedef std::string::const_iterator iterator_type;
    typedef message_parser<iterator_type> message_parser;

    message_parser g;

    const std::string str = ":irc.example.net 001 guest :Welcome to the Internet Relay Network guest!~guest@localhost\n";

    IrcMessage emp;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();

    bool r = parse(iter, end, g, emp);

    if (r && iter == end) {
        std::cout << "Parsing succeeded: " << boost::fusion::as_vector(emp) << std::endl;
    } else {
        std::cout << "Parsing failed\n";
    }

    return 0;
}

/*int main(int, char**) {

    auto client = IrcClient::Create("guest", "Ronald Reagan");
    client->connect("127.0.0.1", 6667);

    std::cin.get();

    client->quit("Gone to have lunch");

    std::cin.get();
}*/
