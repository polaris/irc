#ifndef __ConnectionDelegate_H
#define __ConnectionDelegate_H

class ConnectionDelegate {
public:
    virtual void onConnect() = 0;
    virtual void onError() = 0;
};

#endif  // __ConnectionDelegate_H
