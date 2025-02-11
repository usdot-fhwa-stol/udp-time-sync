#pragma once
#include <thread>
#include <spdlog/spdlog.h>

#include <carma-clock/carma_clock.h>
#include <kafka-client/kafka_client.h>
#include <rapidjson/document.h>

namespace carma_streets_time_sync {
    struct TimeSyncMessage {
        unsigned long timestamp;
        unsigned long timestep;
    };

    class TimeSync {
        private:
            bool initialized = false;
            bool _simulation_mode = false;
            std::thread consumer_thread;
            std::shared_ptr<kafka_clients::kafka_consumer_worker> _time_consumer;          
            std::unique_ptr<fwha_stol::lib::time::CarmaClock> clock;
            std::string get_system_config(const char *config_name, const std::string &default_val) const noexcept;
            void check_initialized();

        public:

            TimeSync();
            void start();
            unsigned long nowInMilliseconds();
            void sleep(unsigned long ms);
            void sleep_until(unsigned long ms);
            unsigned long read_time_sync_message(const std::string &time_sync);

    };

}
