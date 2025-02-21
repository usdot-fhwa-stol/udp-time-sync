#pragma once
#include <thread>
#include <spdlog/spdlog.h>

#include <carma-clock/carma_clock.h>
#include <rapidjson/document.h>
#include <udp-socket/UdpServer.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/async.h> //support for async logging.
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> // or "../stdout_sinks.h" if no colors needed

namespace time_sync {
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
            bool _performance_logging = false;

            std::string _ip;
            unsigned int _port;

            std::thread consumer_thread;
            std::unique_ptr<udp_socket::UdpServer> _time_consumer;          
            std::unique_ptr<fwha_stol::lib::time::CarmaClock> clock;
            /**
             * Loop to consume time sync messages from UDP server and update carma clock
             */
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

    std::string getSystemConfig(const char *config_name, const std::string &default_val) ;


    std::shared_ptr<spdlog::logger> createLogger(const std::string &name, const std::string &extension, const std::string &pattern, const spdlog::level::level_enum &level);
}
