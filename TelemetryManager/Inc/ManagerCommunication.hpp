
#ifndef MANAGERCOMMUNICATION_HPP
#define MANAGERCOMMUNICATION_HPP

#include "ManagerCommunicationBase.hpp"

class ManagerCommunication : ManagerCommunicationBase {
   public:

    void send(mavlink_message_t msg, Destination manager);
    void receive();
};

#endif  // MANAGERCOMMUNICATION_HPP
