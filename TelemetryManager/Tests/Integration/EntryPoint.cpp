#include <iostream>

#include "MockCMSISAbstractor.hpp"
#include "MockIntGroundStationCommunication.hpp"
#include "TelemetryManager.hpp"

int main() {
    const uint8_t RFD900_BUF_SIZE = 11200;

    uint8_t rfd900_buf[RFD900_BUF_SIZE];
    CircularBuffer rfd900_circular_buffer_inst(rfd900_buf, RFD900_BUF_SIZE);
    CircularBuffer* rfd900_circular_buffer = &rfd900_circular_buffer_inst;

    // Struct containing the state of the drone
    TMStateData stateData;

    // Sample values to be assigned to stateData
    int32_t alt = 1000, lat = 500, lon = 300, relative_alt = 50;
    int16_t vx = 10, vy = 5, vz = -3;
    uint16_t hdg = 180;
    int32_t time_boot_ms = 10000;
    float roll = 1.0, pitch = 0.5, yaw = -0.2;
    float rollspeed = 0.1, pitchspeed = 0.05, yawspeed = -0.01;

    // Use memory addresses for stateData, as it uses pointers
    stateData.set_alt(&alt);
    stateData.set_lat(&lat);
    stateData.set_lon(&lon);
    stateData.set_relative_alt(&relative_alt);
    stateData.set_vx(&vx);
    stateData.set_vy(&vy);
    stateData.set_vz(&vz);
    stateData.set_hdg(&hdg);
    stateData.set_time_boot_ms(&time_boot_ms);
    stateData.set_roll(&roll);
    stateData.set_pitch(&pitch);
    stateData.set_yaw(&yaw);
    stateData.set_rollspeed(&rollspeed);
    stateData.set_pitchspeed(&pitchspeed);
    stateData.set_yawspeed(&yawspeed);

    MAV_STATE mavState = MAV_STATE::MAV_STATE_ACTIVE;
    MAV_MODE_FLAG mavMode = MAV_MODE_FLAG::MAV_MODE_FLAG_GUIDED_ENABLED;

    // Creating buffers for GroundStationCommunication parameters
    TMCircularBuffer DMAReceiveBuffer = *(new TMCircularBuffer(rfd900_circular_buffer));
    uint8_t* lowPriorityTransmitBuffer = new uint8_t[RFD900_BUF_SIZE];
    uint8_t* highPriorityTransmitBuffer = new uint8_t[RFD900_BUF_SIZE];

    // Initialize MockCMSIS and other dependencies
    MavlinkTranslator MT;
    MockCMSISAbstractor mockCMSIS;
    std::mutex bufferMutex;

    // Placeholder for GroundStationCommunication mock class instantiation
    MockIntGroundStationCommunication GSC(DMAReceiveBuffer, lowPriorityTransmitBuffer,
                                          highPriorityTransmitBuffer, RFD900_BUF_SIZE, bufferMutex);

    // Initialize TelemetryManager with mock objects
    TelemetryManager* TM = new TelemetryManager(stateData, mavState, mavMode, GSC, MT, mockCMSIS);

    TM->init();

    while (1) {
        // TM->GSC.receive();

        std::cout << "We are here\n";
        {
            std::cout << "Trying to lock bufferMutex in main\n";
            std::lock_guard<std::mutex> lock(bufferMutex);
            std::cout << "Locked bufferMutex in main\n";

            // Decode MAVLink message from the buffer
            // TM->MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);
            TM->update();

            std::cout << "Unlocked bufferMutex in main\n";
        }

        mockCMSIS.CMSISDelay(1000);
    }
    return 0;
}