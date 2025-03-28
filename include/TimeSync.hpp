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
#include <thread>
#include <iostream>
#include <fstream>
#include "ClockSingleton.hpp"
#include <rapidjson/document.h>
#include <udp-socket/UdpServer.hpp>
#include <atomic>
namespace time_sync {
    /**
     * TimeSyncMessage struct
     */
    struct TimeSyncMessage {
        unsigned long timestep;
        unsigned long seq;
    };
    /**
     * TimeSync class
     */
    class TimeSync {
        private:
            bool initialized = false;
            bool _simulation_mode = false;
            bool _performance_logging = false;
            std::atomic<bool> _running = true;

            std::string _ip;
            unsigned int _port;
            bool _debug = false;
            std::ofstream  _performance_log;

            std::thread consumer_thread;
            std::unique_ptr<udp_socket::UdpServer> _time_consumer;          
            /**
             * Loop to consume time sync messages from UDP server and update carma clock
             */
            void consumeTimeLoop();

            void logDebug(const std::string &msg) const;

            void performanceLog(unsigned long real_time, unsigned long carma_time);

        
            /**
             * Get system config value (environment variable)
             * @param config_name Name of the config
             * @param default_val Default value if config is not set
             * @return Value of the config 
             */
            std::string getSystemConfig(const char *config_name, const std::string &default_val) const;

             /**
             * Create a Performance logger that logs real time and carma time
             * @return bool True if logger is created successfully
             */
            bool createPerformanceLogger();

        public:
            /**
             * Constructor for TimeSync object
             * @param ip IP address of the time sync server
             * @param port Port of the time sync server
             */
            TimeSync(const std::string &ip = "127.0.0.1", unsigned int port = 4567, bool debug = false);

            ~TimeSync();
            /**
             * If in simulation mode, starts independent thread to consume time sync messages and update carma-clock
             */
            void start();
            
            /**
             * Stop the time sync thread
             */
            void stop();

            /**
             * Read time sync message
             * @param time_sync Time sync message
             * @return Timestamp of the time sync message
             */
            TimeSyncMessage readTimeSyncMessage(const std::string &time_sync) const;

    };
    /**
     * Get current time in milliseconds
     * @return Current time in milliseconds
     */
    unsigned long nowInMilliseconds();
    /**
     * Block thread for given time (ms)
     * @param ms Time to sleep in milliseconds
     */
    void sleep(unsigned long ms);
    /**
     * Block thread until given time (ms)
     * @param ms Future time in milliseconds
     */
    void sleepUntil(unsigned long ms);
    

   
}
