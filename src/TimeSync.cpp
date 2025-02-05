#include "TimeSync.hpp"

namespace carma_streets_time_sync {
    TimeSync::TimeSync() : initialized(false) {}

    void TimeSync::start() {
        if (!initialized) {
            // Intialize carma clock
            std::string sim_mode_string = get_system_config("SIMULATION_MODE","TRUE");
            _simulation_mode = sim_mode_string.compare("true") == 0 || sim_mode_string.compare("TRUE") == 0 ;
            clock = std::make_unique<fwha_stol::lib::time::CarmaClock>(_simulation_mode);
            if ( _simulation_mode) {
                // Initalize kafka consumer
                std::string kafka_broker = get_system_config("KAFKA_BROKER","localhost:9092");
                std::string kafka_topic = get_system_config("KAFKA_TOPIC","time_sync");
                std::string kafka_group_id = get_system_config("KAFKA_GROUP_ID","mmitss_time_sync");
                auto kafka_clients = kafka_clients::kafka_client();
                _time_consumer = kafka_clients.create_consumer(kafka_broker, kafka_topic, kafka_group_id);
                _time_consumer->init();
                _time_consumer->subscribe();
                _time_consumer->printCurrConf(); 
                consumer_thread = std::thread([this]() {
                    while (_time_consumer->is_running()) {
                        std::string time_sync = _time_consumer->consume(1000);
                        SPDLOG_INFO("TimeSync message: {0}", time_sync);
                        if (!initialized && !time_sync.empty()) {
                            initialized = true;
                            SPDLOG_INFO("TimeSync initialized!");
                        }
                    }
                    if ( !_time_consumer->is_running()) {
                        SPDLOG_ERROR("Kafka time sync consumer is not running!");
                        throw std::runtime_error("Kafka time sync consumer is not running!");   
                    }   
                });
            }
        }
    }

    unsigned long TimeSync::now() {
        return clock->nowInMilliseconds();
    }
    void TimeSync::check_initialized() {
        if (!initialized) {
            SPDLOG_ERROR("TimeSync is not initialized!");
            throw std::runtime_error("TimeSync is not initialized!");
        }
    }
    void TimeSync::sleep(unsigned long ms) {
        clock->sleep_for(ms);
    }

    void TimeSync::sleep_until(unsigned long ms) {
        clock->sleep_until(ms);
    }

    std::string TimeSync::get_system_config(const char *config_name, const std::string &default_val) const noexcept{
        // Check for config_name nullptr and use default value
        if (config_name == nullptr ) {
            SPDLOG_WARN("System config_name was nullptr! Using default value {0}!" , default_val);
            return default_val;
        }
        // If std::getenv(config_name) returns value, use this value
        if (const auto config = std::getenv(config_name)) {
            SPDLOG_DEBUG("Reading system config {0} as : {1}!", config_name, config);
            return config;
        }
        // If std::getenv(config_name) returns nullptr, environment variable was not set so use default
        else {
            SPDLOG_WARN("System config {0} was not set! Using default value {1}!" ,config_name, default_val);
            return default_val;
        }
    }
}
