// CMSISAbstractorBase.hpp
#ifndef CMSIS_ABSTRACTOR_BASE_HPP
#define CMSIS_ABSTRACTOR_BASE_HPP

#include <cstdint>

class CMSISAbstractorBase {
public:
    virtual ~CMSISAbstractorBase() = default;

    // Pure virtual functions that subclasses must implement
    virtual void CMSISDelay(const uint64_t ms) = 0;
    virtual void CMSISCreateTask(void (*exeFunction)(void*), void* context, const char* pcName) = 0;
};

#endif // CMSIS_ABSTRACTOR_BASE_HPP
