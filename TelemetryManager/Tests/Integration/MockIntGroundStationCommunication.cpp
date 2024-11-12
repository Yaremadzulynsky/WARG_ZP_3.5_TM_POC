#include "MockIntGroundStationCommunication.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>

constexpr int DEFAULT_PORT = 14550;
constexpr char DEFAULT_IP[] = "127.0.0.1";

MockIntGroundStationCommunication::MockIntGroundStationCommunication(
    TMCircularBuffer &DMAReceiveBuffer, uint8_t *lowPriorityTransmitBuffer,
    uint8_t *highPriorityTransmitBuffer, int length, std::mutex &bufferMutex)
    : GroundStationCommunicationBase(DMAReceiveBuffer,
                                     TMCircularBuffer(lowPriorityTransmitBuffer, length),
                                     TMCircularBuffer(highPriorityTransmitBuffer, length)), bufferMutex(bufferMutex) {
    setupUDPSocket(DEFAULT_IP, DEFAULT_PORT);

    // Start the receive thread to listen for incoming UDP data
    receiveThread = std::thread(&MockIntGroundStationCommunication::receive, this);
    receiveThread.detach();  // Detach thread to run independently
}

MockIntGroundStationCommunication::~MockIntGroundStationCommunication() {
    if (udp_socket >= 0) {
        close(udp_socket);
    }
    if (receiveThread.joinable()) {
        receiveThread.join();  // Ensure thread is joined before destruction (if not detached)
    }
}

void MockIntGroundStationCommunication::setupUDPSocket(const char *ip, int port) {
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        std::cerr << "Failed to create UDP socket" << std::endl;
        return;
    }

    std::memset(&target_address, 0, sizeof(target_address));
    target_address.sin_family = AF_INET;
    target_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target_address.sin_addr);
}

void MockIntGroundStationCommunication::receive() {
    uint8_t receiveBuffer[2048];
    sockaddr_in sender_address;
    socklen_t sender_address_len = sizeof(sender_address);

    while (true) {
        ssize_t received_bytes = recvfrom(udp_socket, receiveBuffer, sizeof(receiveBuffer), 0,
                                          (sockaddr *)&sender_address, &sender_address_len);
       if (received_bytes > 0) {
            std::cout << "Received " << received_bytes << " bytes from UDP\n";

            // Lock the mutex and log entry
            std::cout << "receive(): Trying to lock bufferMutex\n";
            std::lock_guard<std::mutex> lock(bufferMutex);
            std::cout << "receive(): bufferMutex locked\n";

            for (ssize_t i = 0; i < received_bytes; ++i) {
                if (!DMAReceiveBuffer.enqueue(receiveBuffer[i])) {
                    std::cerr << "DMAReceiveBuffer is full. Dropping data.\n";
                    break;
                }
            }

            // Log exit from critical section
            std::cout << "receive(): bufferMutex unlocked\n";
        } else if (received_bytes < 0) {
            std::cerr << "Failed to receive UDP packet" << std::endl;
        }
    }
}

void MockIntGroundStationCommunication::transmit(TMCircularBuffer &transmissionBuffer) {
    const uint8_t RFD900_BUF_SIZE = 11200;

    while (transmissionBuffer.messagesInQueue >= 0) {
        int bytesToTransmit = transmissionBuffer.bytesUntilMessageEnd();

        if (bytesToTransmit > RFD900_BUF_SIZE) {
            bytesToTransmit = RFD900_BUF_SIZE;
        }

        for (int i = 0; i < bytesToTransmit; ++i) {
            internalBuffer_[i] = transmissionBuffer.dequeue();
        }

        int sent_bytes = sendto(udp_socket, internalBuffer_, bytesToTransmit, 0,
                                (sockaddr *)&target_address, sizeof(target_address));
        if (sent_bytes < 0) {
            std::cerr << "Failed to send UDP packet" << std::endl;
        } else {
            std::cout << "Sent " << sent_bytes << " bytes to ground station via UDP\n";
        }

        transmissionBuffer.messagesInQueue--;
    }
}
