
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size) : CircularBuffer(buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer* buf) : CircularBuffer(*buf) {
    // Constructor
}

TMCircularBuffer::~TMCircularBuffer() {
    // Destructor
}

TMCircularBuffer::MAVLinkByte TMCircularBuffer::dequeue(bool* success) {
    MAVLinkByte res = 0;
    if (read(&res, 1)) {
        if (success != nullptr) {
            *success = true;
        }
        return res;
    }
    if (success != nullptr) {
        *success = false;
    }
    return 0;
}

bool TMCircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
    if (write(byte)) {
        return true;
    }
    return false;
}

bool TMCircularBuffer::enqueue(MAVLinkByte* bytes, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (!enqueue(bytes[i])) {
            return false;
        }
    }
    return true;
}

// int TMCircularBuffer::bytesUntilMessageEnd(bool* success) {
//     int index = 0;
//     uint8_t byte = 0;
//     uint16_t size = writePtr_ - readPtr_;

//     while (index < size) {
//         if (peek(byte, index)) {
//             if (byte == 0xFD) {  // 0xFD is the start flag for a MAVLink message
//                 if (success != nullptr) {
//                     *success = true;
//                 }
//                 if (index != 0) {
//                     return index;
//                 }
//             }
//         }
//         index++;
//     }
//     if (success != nullptr) {
//         *success = false;
//     }
//     return index;
// }

int TMCircularBuffer::bytesUntilMessageEnd(bool* success) {
    int index = 0;
    uint8_t byte = 0;
    uint16_t size = writePtr_ - readPtr_;
    const int mavlinkHeaderLength = 10;  // MAVLink 2.0 minimum header length (includes 2 CRC bytes)

    while (index < size) {
        if (peek(byte, index)) {
            // Check if this byte is the MAVLink 2.0 start byte
            if (byte == 0xFD) {  // Assuming 0xFD indicates the start of a MAVLink message

                // Check if there's enough space for the minimum header
                if (index + mavlinkHeaderLength > size) {
                    if (success) *success = false;  // Incomplete message
                    return index + mavlinkHeaderLength;
                }

                // Peek at the length byte to determine payload size
                uint8_t payloadLength = 0;
                if (peek(payloadLength, index + 1)) {  // Length byte is at index+1

                    int totalMessageLength = mavlinkHeaderLength + payloadLength;
                    
                    // Check for CRC (2 bytes at end of MAVLink 2.0 packet)
                    totalMessageLength += 2;

                    // Ensure we have the entire packet in the buffer
                    if (index + totalMessageLength <= size) {
                        if (success) *success = true;
                        return index + totalMessageLength;  // Return total bytes to end of packet
                    }
                }
            }
        }
        index++;
    }
    
    if (success) *success = false;
    return size;  // No complete packet found; return available buffer size
}



uint16_t TMCircularBuffer::currentIndex() { return readPtr_; }
