#pragma once
#include <thread>
#include <spdlog/spdlog.h>

#include <carma-clock/carma_clock.h>
#include <rapidjson/document.h>
#include <udp-socket/UdpServer.hpp>
#include <atomic>

namespace carma_streets_time_sync {
    /**
     * TimeSyncMessage struct
     */
    struct TimeSyncMessage {
        unsigned long timestamp;
        unsigned long timestep;
    };
    /**
     * TimeSync class
     */
    class TimeSync {
        private:
            bool initialized = false;
            bool _simulation_mode = false;

            std::string _ip;
            unsigned int _port;

            std::thread consumer_thread;
            std::unique_ptr<udp_socket::UdpServer> _time_consumer;          
            std::unique_ptr<fwha_stol::lib::time::CarmaClock> clock;
            std::string getSystemConfig(const char *config_name, const std::string &default_val) const noexcept;
            void consumeTimeLoop();

        public:
            /**
             * Constructor for TimeSync object
             * @param ip IP address of the time sync server
             * @param port Port of the time sync server
             */
            TimeSync(const std::string &ip = "127.0.0.1", unsigned int port = 4567);
            /**
             * If in simulation mode, starts independent thread to consume time sync messages and update carma-clock
             */
            void start();
            /**
             * Get current time in milliseconds
             * @return Current time in milliseconds
             */
            unsigned long nowInMilliseconds() const;
            /**
             * Method will block thread for given time (ms)
             * @param ms Time to sleep in milliseconds
             */
            void sleep(unsigned long ms);
            /**
             * Method will block thread until given time (ms)
             * @param ms Future time in milliseconds
             */
            void sleepUntil(unsigned long ms);
            /**
             * Read time sync message
             * @param time_sync Time sync message
             * @return Timestamp of the time sync message
             */

    };
    TimeSyncMessage readTimeSyncMessage(const std::string &time_sync);

}
