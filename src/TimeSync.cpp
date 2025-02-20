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
                SPDLOG_INFO("TimeSync is in simulation mode!");
                _time_consumer= std::make_unique<udp_socket::UdpServer>("127.0.0.1", 4567 );
                try {
                    consumer_thread = std::thread([this]() {
                        try {
                            while (true) {
                                SPDLOG_INFO("Inside thread");
                                std::string time_sync = _time_consumer->stringTimedReceive(100);
                                SPDLOG_INFO("Timed Received");

                                SPDLOG_INFO("TimeSync message: {0}", time_sync);
                                if (!time_sync.empty()) {
                                    if (!initialized) {
                                        initialized = true;
                                    
                                    SPDLOG_INFO("TimeSync initialized!");
                                    }
                                    clock->update(read_time_sync_message(time_sync));
                                }
                            }
                        }
                        catch (const std::exception &e) {
                            SPDLOG_ERROR("TimeSync consumer thread exception: {0}", e.what());
                        }
                
                    });
                    consumer_thread.detach();
                }
                catch (const std::exception &e) {
                    SPDLOG_ERROR("TimeSync consumer thread exception: {0}", e.what());
                }
            }
        }
    }

    unsigned long TimeSync::nowInMilliseconds() {
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

    unsigned long TimeSync::read_time_sync_message(const std::string &time_sync) {
        TimeSyncMessage message;
         rapidjson::Document document;
        document.Parse(time_sync.c_str());

        if (document.HasParseError()) {
            throw std::runtime_error( "Parsing error: " + document.GetParseError() );
        }
        if (!document.IsObject() && !document.HasMember("timestamp")) {
            throw std::runtime_error( "Parsing error: time sync message " + time_sync + " is not a valid time sync message!" );    
        }
        return document["timestamp"].GetUint64();
    }

}
