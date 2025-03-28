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
    TimeSync::TimeSync(const std::string &ip , unsigned int port, bool debug ) : initialized(false), _ip(ip), _port(port), _debug(debug) ,_performance_log() {
        
    }

    TimeSync::~TimeSync() {
        stop();
        if ( _performance_log && _performance_log.is_open()) {
            _performance_log.close();
        }
    }
 
    void TimeSync::start() {
        if (!initialized) {
            // Intialize carma clock
            std::string sim_mode_string = getSystemConfig("SIMULATION_MODE","FALSE");
            _simulation_mode = sim_mode_string.compare("true") == 0 || sim_mode_string.compare("TRUE") == 0 ;

            
            ClockSingleton::create(_simulation_mode);
            if ( _simulation_mode) {
                std::string performance_logging = getSystemConfig("PERFORMANCE_LOGGING","FALSE");
                _performance_logging = performance_logging.compare("true") == 0 || performance_logging.compare("TRUE") == 0 ;
                if (_performance_logging) {
                    logDebug("Performance logging is enabled!");
                    if (!createPerformanceLogger()) {
                        logDebug("Failed to create performance logger!");
                    }
                }
                // Initalize kafka consumer
                logDebug("TimeSync is in simulation mode!");
                _time_consumer= std::make_unique<udp_socket::UdpServer>(_ip, _port);
                consumer_thread = std::thread(&TimeSync::consumeTimeLoop, this);
                consumer_thread.detach();
            }
        }
    }

    void TimeSync::logDebug(const std::string &msg) const{
        if (_debug) {
            std::cout << "[DEBUG]"  <<  msg << std::endl;
        }
    }

    void TimeSync::performanceLog(unsigned long real_time, unsigned long carma_time) {
        if (_performance_logging && _performance_log && _performance_log.is_open()) {
            _performance_log << std::to_string(real_time) << "," << std::to_string(carma_time) << std::endl;
            _performance_log.flush();
        }
    }
    void TimeSync::consumeTimeLoop() {
        while (_running) {
            std::string time_sync = _time_consumer->stringTimedReceive(100);
            logDebug("TimeSync message received : " + time_sync);
            if (!time_sync.empty()) {
                if (!initialized) {
                    initialized = true;
                    logDebug("TimeSync initialized!");
                }
                auto message = readTimeSyncMessage(time_sync);
                ClockSingleton::update(message.timestep);
                if (_performance_logging) {
                    // Log Real Time and CARMA Time 
                    performanceLog(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), nowInMilliseconds());
                }
            }
        }
    }
    
    void TimeSync::stop() {
        _running = false;
        if (consumer_thread.joinable()) {
            consumer_thread.join();
        }
    }
    unsigned long nowInMilliseconds(){
        return ClockSingleton::time_in_ms();
    }

    void sleep(unsigned long ms) {
        ClockSingleton::sleep_for(ms);
    }

    void sleepUntil(unsigned long ms) {
        ClockSingleton::sleep_until(ms);
    }
    std::string TimeSync::getSystemConfig(const char *config_name, const std::string &default_val) const {
        // Check for config_name nullptr and use default value
        if (config_name == nullptr ) {
            logDebug("System config_name was nullptr! Using default value " + default_val);
            return default_val;
        }
        // If std::getenv(config_name) returns value, use this value
        if (const auto config = std::getenv(config_name)) {
            logDebug("Reading system config " + std::string(config_name) + " as : " + std::string(config));
            return config;
        }
        // If std::getenv(config_name) returns nullptr, environment variable was not set so use default
        else {
            logDebug("System config " + std::string(config_name) + " was not set! Using default value " + default_val);
            return default_val;
        }
    }

    TimeSyncMessage TimeSync::readTimeSyncMessage(const std::string &time_sync) const{
        TimeSyncMessage message;
        rapidjson::Document document;
        document.Parse(time_sync.c_str());
        logDebug("Parsing time sync message: " + time_sync);
        if (document.HasParseError()) {
            throw std::runtime_error( "Parsing error: " + document.GetParseError() );
        }
        if (!document.IsObject() || !document.HasMember("timestep") || !document["timestep"].IsUint64()) {
            throw std::runtime_error( "Parsing error: time sync message " + time_sync + " is not a valid time sync message!" );    
        }
        message.timestep =  document["timestep"].GetUint64();
        if (!document.IsObject() || !document.HasMember("seq") || !document["seq"].IsUint64()) {
            throw std::runtime_error( "Parsing error: time sync message " + time_sync + " is not a valid time sync message!" );    
        }
        message.seq =  document["seq"].GetUint64();
        return message;

    }
    bool TimeSync::createPerformanceLogger(){
        std::string log_dir = getSystemConfig("LOGS_DIR","/opt/carma/logs/");
        if (!std::filesystem::exists(log_dir)) {
            if (!std::filesystem::create_directories(log_dir)) {
                std::cerr << "Error creating directory: " << log_dir << std::endl;
                return false;
            }
        }
        std::string date_time = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        std::string log_file = log_dir + "performance_" + date_time + ".csv";
        _performance_log.open(log_file);
        if (!_performance_log.is_open()) {
            logDebug("Failed to open file " + log_file);
            // Check for specific error flags
            if (_performance_log.fail()) {
                std::cerr << "Logical error on i/o operation" << std::endl;
            } else if (_performance_log.bad()) {
                std::cerr << "Read/writing error on i/o operation" << std::endl;
            } else if (_performance_log.eof()) {
                std::cerr << "End of file reached" << std::endl;
            }
            perror(log_file.c_str());

            return false;
        }
        _performance_log << "Real Time (ms), Carma Time (ms)" << std::endl;
        _performance_log.flush();  
        return true;
    }

}
