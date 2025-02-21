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
#include "TimeSync.hpp"

namespace time_sync {
    TimeSync::TimeSync(const std::string &ip , unsigned int port ) : initialized(false), _ip(ip), _port(port)  {}
 
    void TimeSync::start() {
        if (!initialized) {
            // Intialize carma clock
            std::string sim_mode_string = getSystemConfig("SIMULATION_MODE","FALSE");
            _simulation_mode = sim_mode_string.compare("true") == 0 || sim_mode_string.compare("TRUE") == 0 ;

            
            clock = std::make_unique<fwha_stol::lib::time::CarmaClock>(_simulation_mode);
            if ( _simulation_mode) {
                std::string performance_logging = getSystemConfig("PERFORMANCE_LOGGING","FALSE");
                _performance_logging = performance_logging.compare("true") == 0 || performance_logging.compare("TRUE") == 0 ;
                if (_performance_logging) {
                    SPDLOG_DEBUG("Performance logging is enabled!");
                    auto logger = createLogger("performance", ".csv", "%v", spdlog::level::info);
                    logger->info("Real Time (ms), Carma Time (ms)");
                }
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
                auto message = readTimeSyncMessage(time_sync);
                clock->update(message.timestamp);
                if (_performance_logging) {
                    if(auto logger = spdlog::get("performance"); logger != nullptr ){
                        logger->info("{0},{1}", 
                            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), 
                            clock->nowInMilliseconds());
                    }
                }
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

    std::string getSystemConfig(const char *config_name, const std::string &default_val) {
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
    std::shared_ptr<spdlog::logger> createLogger(const std::string &name, const std::string &extension, const std::string &pattern, const spdlog::level::level_enum &level){
        std::string log_dir = getSystemConfig("LOGS_DIR","/opt/carma/logs/");
        std::string date_time = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        auto logger  = spdlog::basic_logger_mt<spdlog::async_factory>(
                name,  // logger name
                log_dir + name +date_time + extension  // log file name and path
            );
        // Only log log statement content
        logger->set_pattern(pattern);
        logger->set_level(level);
        logger->flush_on(level);
        return logger;
    }

}
