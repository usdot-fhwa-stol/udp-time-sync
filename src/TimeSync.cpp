#include "TimeSync.hpp"

namespace carma_streets_time_sync {
    TimeSync::TimeSync(const std::string &ip , unsigned int port ) : initialized(false), _ip(ip), _port(port)  {}
 
    void TimeSync::start() {
        if (!initialized) {
            // Intialize carma clock
            std::string sim_mode_string = getSystemConfig("SIMULATION_MODE","TRUE");
            _simulation_mode = sim_mode_string.compare("true") == 0 || sim_mode_string.compare("TRUE") == 0 ;
            clock = std::make_unique<fwha_stol::lib::time::CarmaClock>(_simulation_mode);
            if ( _simulation_mode) {
                // Initalize kafka consumer
                SPDLOG_INFO("TimeSync is in simulation mode!");
                _time_consumer= std::make_unique<udp_socket::UdpServer>(_ip, _port);
                consumer_thread = std::thread([this]() {
                    
                    consumeTimeLoop();                        
                    
                });
                consumer_thread.detach();
            }
        }
    }
    void TimeSync::consumeTimeLoop() {
        while (true) {
            std::string time_sync = _time_consumer->stringTimedReceive(100);
            SPDLOG_DEBUG("TimeSync message: {0}", time_sync);
            if (!time_sync.empty()) {
                if (!initialized) {
                    initialized = true;
                    SPDLOG_DEBUG("TimeSync initialized!");
                }
                clock->update(readTimeSyncMessage(time_sync).timestamp);
            }
        }
    }
    unsigned long TimeSync::nowInMilliseconds() const{
        clock->wait_for_initialization();
        return clock->nowInMilliseconds();
    }

    void TimeSync::sleep(unsigned long ms) {
        clock->wait_for_initialization();
        clock->sleep_for(ms);
    }

    void TimeSync::sleepUntil(unsigned long ms) {
        clock->wait_for_initialization();
        clock->sleep_until(ms);
    }

    std::string TimeSync::getSystemConfig(const char *config_name, const std::string &default_val) const noexcept{
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

    TimeSyncMessage readTimeSyncMessage(const std::string &time_sync) {
        TimeSyncMessage message;
        rapidjson::Document document;
        document.Parse(time_sync.c_str());
        SPDLOG_INFO("Parsing time sync message: {0}", time_sync);
        if (document.HasParseError()) {
            throw std::runtime_error( "Parsing error: " + document.GetParseError() );
        }
        if (!document.IsObject() || !document.HasMember("timestamp") || !document["timestamp"].IsUint64()) {
            throw std::runtime_error( "Parsing error: time sync message " + time_sync + " is not a valid time sync message!" );    
        }
        message.timestamp =  document["timestamp"].GetUint64();
        if (!document.IsObject() || !document.HasMember("timestep") || !document["timestep"].IsUint64()) {
            throw std::runtime_error( "Parsing error: time sync message " + time_sync + " is not a valid time sync message!" );    
        }
        message.timestep =  document["timestep"].GetUint64();
        return message;

    }

}
