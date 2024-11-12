// MockCMSISAbstractor.hpp
#ifndef MOCK_CMSIS_ABSTRACTOR_HPP
#define MOCK_CMSIS_ABSTRACTOR_HPP

#include "CMSISAbstractorBase.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <functional>

class MockCMSISAbstractor : public CMSISAbstractorBase {
private:
    // Vector to store running threads
    std::vector<std::thread> taskThreads;

public:
    // Mock implementation of CMSISDelay with real delay
    void CMSISDelay(const uint64_t ms) override {
        // std::cout << "[MockCMSISAbstractor] Simulated delay for " << static_cast<int>(ms) << " ms\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    // Mock implementation of CMSISCreateTask using std::thread
    void CMSISCreateTask(void (*exeFunction)(void*), void* context, const char* pcName) override {
        std::cout << "[MockCMSISAbstractor] Creating mock task with name: " << pcName << "\n";
        
        // Create and start a new thread for the task function
        taskThreads.emplace_back([exeFunction, context, pcName]() {
            std::cout << "[MockCMSISAbstractor] Starting mock task: " << pcName << "\n";
            exeFunction(context);  // Run the task function in this thread
            std::cout << "[MockCMSISAbstractor] Task finished: " << pcName << "\n";
        });
    }

    // Wait for all tasks to complete (join all threads)
    void waitForAllTasks() {
        for (auto& thread : taskThreads) {
            if (thread.joinable()) {
                thread.join();  // Wait for each thread to finish
            }
        }
        std::cout << "[MockCMSISAbstractor] All tasks completed.\n";
    }

    // Destructor to clean up all threads if they haven't finished
    ~MockCMSISAbstractor() {
        for (auto& thread : taskThreads) {
            if (thread.joinable()) {
                thread.join();  // Ensure all threads are joined before destruction
            }
        }
    }
};

#endif // MOCK_CMSIS_ABSTRACTOR_HPP
