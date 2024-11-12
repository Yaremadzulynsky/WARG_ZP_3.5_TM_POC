
#ifndef MANAGERCOMMUNICATIONBASE_HPP
#define MANAGERCOMMUNICATIONBASE_HPP

#include <cstdint>
#include "mavlink.h"

class ManagerCommunicationBase {
   public:
    enum Destination {
        PATH_MANAGER,
        SYSTEM_MANAGER,
    };

    virtual void send(mavlink_message_t msg, Destination manager) = 0;
    virtual void receive() = 0;
};

#endif  // MANAGERCOMMUNICATIONBASE_HPP
