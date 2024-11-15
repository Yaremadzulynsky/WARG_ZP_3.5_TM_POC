#include "MavlinkTranslator.hpp"

MavlinkTranslator::MavlinkTranslator() {
    // Constructor
}

MavlinkTranslator::~MavlinkTranslator() {
    // Destructor
}

void MavlinkTranslator::bytesToMavlinkMsg(TMCircularBuffer &rxFromGroundByteQueue) {
    // Take bytes in rxFromGroundByteQueue and convert them to Mavlink messages
    bool success = true;
    uint8_t bytesUntilEnd = 0;
    std::cout << "Decoding messages from the ground station\n";
    while (success) {
        bytesUntilEnd = rxFromGroundByteQueue.bytesUntilMessageEnd(&success);

        if (success) {
            if (bytesUntilEnd != 0) {
                uint8_t rxByte[bytesUntilEnd];

                for (uint8_t i = 0; i < bytesUntilEnd; i++) {
                    rxByte[i] = rxFromGroundByteQueue.dequeue();
                }
                decoder.parseBytesToMavlinkMsgs(rxByte, bytesUntilEnd);
                decodedMessages++;
            }
        }
    }
}

void MavlinkTranslator::addMavlinkMsgToByteQueue(mavlink_message_t &msg,
                                                 TMCircularBuffer &txToGroundByteQueue) {
    encoder.encodeMsgIntoBuffer(msg, txToGroundByteQueue);
    txToGroundByteQueue.messagesInQueue++;
}