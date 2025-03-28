/**
 * Copyright (C) 2025 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include "ClockSingleton.hpp"

namespace time_sync {

    void ClockSingleton::update( uint64_t timestep) {
        auto &inst = getSingleton();
        inst.update(timestep);
    }

    uint64_t ClockSingleton::time_in_ms() {
        auto &inst = getSingleton();
        inst.wait_for_initialization();
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
