// CMSISAbstractor.hpp
#ifndef CMSIS_ABSTRACTOR_HPP
#define CMSIS_ABSTRACTOR_HPP

#include <FreeRTOS.h>
#include <task.h>

#include "CMSISAbstractorBase.hpp"

class CMSISAbstractor : public CMSISAbstractorBase {
    //    TaskHandle_t routineDataTransmissionH = NULL

   public:
    // Real implementation of CMSISDelay
    void CMSISDelay(const uint64_t ms) override {
        // Real delay functionality, e.g., using an RTOS delay function
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

    // Real implementation of CMSISCreateTask
    void CMSISCreateTask(void (*exeFunction)(void*), void* context, const char* pcName) override {
        // Real task creation functionality, e.g., using FreeRTOS task creation
        TaskHandle_t taskHandle = nullptr;
        xTaskCreate(exeFunction, pcName, 512UL, context, 24, &taskHandle);
    }
};

#endif  // CMSIS_ABSTRACTOR_HPP
