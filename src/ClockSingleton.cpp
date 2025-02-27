#include "ClockSingleton.hpp"

namespace time_sync {

    void ClockSingleton::update( uint64_t timestep) {
        auto &inst = getSingleton();
        inst.update(timestep);
        SPDLOG_TRACE("streets clock is updated to {0}", timestep);
    }

    uint64_t ClockSingleton::time_in_ms() {
        auto &inst = getSingleton();
        SPDLOG_TRACE("Calling thread is waiting on streets clock initialization.");
        inst.wait_for_initialization();
        SPDLOG_TRACE("Thread is released after initialization.");
        return inst.nowInMilliseconds();
    }

    void ClockSingleton::sleep_for(uint64_t ms ) {
        auto &inst = getSingleton();
        inst.wait_for_initialization();
        auto cur_time = inst.nowInMilliseconds();
        inst.sleep_until(cur_time + ms);
    }

    void ClockSingleton::sleep_until(uint64_t ms) {
        auto &inst = getSingleton();
        inst.wait_for_initialization();
        inst.sleep_until(ms);
    }
}
