
#include "ManagerCommunication.hpp"

#include <iostream>

void ManagerCommunication::send(mavlink_message_t msg, Destination manager) {
    // Send the message to the appropriate manager
    std::cout << "Sending message to manager" << std::endl;
    // print the manager that the message is being sent to
    switch (manager) {
        case Destination::PATH_MANAGER:
            std::cout << "PATH_MANAGER" << std::endl;
            break;
        case Destination::SYSTEM_MANAGER:
            std::cout << "SYSTEM_MANAGER" << std::endl;
            break;
    }
}

void ManagerCommunication::receive() {
    // Receive messages from the managers
}