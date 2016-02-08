#ifndef __ConnectionDelegate_H
#define __ConnectionDelegate_H

#include <string>

class ConnectionDelegate {
public:
    virtual ~ConnectionDelegate() = default;
    virtual void onConnect() = 0;
    virtual void onMessage(const std::string &message) = 0;
    virtual void onError() = 0;
};

#endif  // __ConnectionDelegate_H
