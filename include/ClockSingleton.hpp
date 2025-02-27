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
#pragma once
#include "Singleton.hpp"
#include <spdlog/spdlog.h>
#include <carma-clock/carma_clock.h>

namespace time_sync {
    /**
     * @brief Streets Clock Singleton is a singleton that manages a CarmaClock object. The CarmaClock
     * object is created by the CARMA Time Library (https://github.com/usdot-fhwa-stol/carma-time-lib)
     * and is a wrapper object that allows for time based operation using chronos system time calls or
     * an external source for time. Its constructor takes a boolean parameter, what when set to true 
     * relies on update calls to save and store the current time. When passed false, the CarmaClock object
     * functions exactly as chrono system time calls including now() and sleep() calls, using the system time.
     * 
     * **NOTE** Before attempting to statically access any functionality in this class, call 
     * ClockSingleton::create(bool simulation_mode) to initialize the clock singleton in simulation mode
     * or real time mode.
     * 
     * 
     * @author Paul Bourelly
     */
    class ClockSingleton : public Singleton<fwha_stol::lib::time::CarmaClock, bool> {
        friend class Singleton<fwha_stol::lib::time::CarmaClock, bool>;

        public:
            /**
             * @brief Static access to the CarmaClock update method (see https://github.com/usdot-fhwa-stol/carma-time-lib).
             * Updates the current time held by the CarmaClock to the value of the time parameter (in milliseconds).
             * 
             * @throw std::invalid_arguement if called with CarmaClock not in simulation mode
             * @param time time to update to in milliseconds.
             */
            static void update( uint64_t time );
            /**
             * @brief Static access to the CarmaClock timeInMilliseconds method (see https://github.com/usdot-fhwa-stol/carma-time-lib).
             * Returns the epoch time in milliseconds. 
             * 
             * @return In simulation mode this returns the value from the last update call. 
             * If update has not been called yet this method will block until the first call to update and then return that value. 
             * When not in simulation mode return system epoch time in milliseconds.
             */
            static uint64_t time_in_ms();
            /**
             * @brief Sleep/block calling thread for provided duration in milliseconds. 
             * @param ms time in milliseconds to sleep for.
             */
            static void sleep_for(uint64_t ms);
            /**
             * @brief Sleep/block calling thread until provided time in milliseconds. 
             * @param ms time in milliseconds to sleep until.
             */
            static void sleep_until(uint64_t ms);


        protected:
            // Hide getSingleton method. Use static methods instead.
            using Singleton::getSingleton;


    };
}
