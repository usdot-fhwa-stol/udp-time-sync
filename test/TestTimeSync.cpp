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
#include <gtest/gtest.h>
#include <TimeSync.hpp>
#include <udp-socket/UdpClient.hpp>
#include <ClockSingleton.hpp>

#include <chrono>

TEST(TimeSync, now) {
    setenv("SIMULATION_MODE", "FALSE", 1);

    time_sync::TimeSync sync;
    sync.start();  
    EXPECT_NEAR(time_sync::nowInMilliseconds() , 
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(),
        100); 
    sync.stop();

}

TEST(TimeSync, readTimeSyncMessage) {

    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    auto timeSyncMessage = time_sync::readTimeSyncMessage(time_sync_message);
    EXPECT_EQ(timeSyncMessage.timestamp, 1231);
    EXPECT_EQ(timeSyncMessage.timestep, 1000);
}

TEST(TimeSync, readTimeSyncMessageInvalid) {

    std::string time_sync_message = "{\"something\": 1231, \"timestep\": 1000}";
    EXPECT_THROW( time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
    time_sync_message = "{\"timestamp\": 1231, \"something\": 1000}";
    EXPECT_THROW( time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
    time_sync_message = "invalidJson";
    EXPECT_THROW( time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
}

TEST(TimeSync, testStart) {
    setenv("SIMULATION_MODE", "TRUE", 1);
    setenv("PERFORMANCE_LOGGING", "TRUE", 1);

    time_sync::TimeSync sync;
    sync.start();  
    udp_socket::UdpClient client("127.0.0.1", 4567);
    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    client.Send(time_sync_message);
    EXPECT_EQ(time_sync::nowInMilliseconds(), 1231); 
    EXPECT_NO_THROW(time_sync::sleepUntil(1231));
    sync.stop();
    // time for thread cleanup
    sleep(1);

}
