#ifndef CALLBACKINTERFACE_H
#define CALLBACKINTERFACE_H
#include <vector>

class CallbackInterface {
          virtual void cardcallback(int dealer, std::vector<int> playercards) = 0;
          
};


#endif //CALLBACKINTERFACE_H