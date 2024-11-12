// GroundStationCommunicationBase.hpp
#ifndef GROUNDSTATIONCOMMUNICATIONBASE_HPP
#define GROUNDSTATIONCOMMUNICATIONBASE_HPP

#include <cstdint>

#include "TMCircularBuffer.hpp"

class GroundStationCommunicationBase {
   public:
    /*
     * When the DMA interrupt is triggered the data should be stored in the DMAReceiveBuffer
     * IF there is space.
     */
    TMCircularBuffer& DMAReceiveBuffer;

    // low priority/Non routine Mavlink bytes to be sent to the ground station.
    TMCircularBuffer lowPriorityTransmitBuffer;

    GroundStationCommunicationBase(TMCircularBuffer& dmaBuffer, TMCircularBuffer lowPriorityBuffer,
                                   TMCircularBuffer highPriorityBuffer)
        : DMAReceiveBuffer(dmaBuffer),
          lowPriorityTransmitBuffer(lowPriorityBuffer),
          highPriorityTransmitBuffer(highPriorityBuffer) {}

    // high priority/Routine Mavlink bytes to be sent to the ground station.
    TMCircularBuffer highPriorityTransmitBuffer;
    virtual ~GroundStationCommunicationBase() = default;
    virtual void transmit(TMCircularBuffer& transmissionBuffer) = 0;
    virtual void receive() = 0;
};

#endif  // GROUNDSTATIONCOMMUNICATIONBASE_HPP
