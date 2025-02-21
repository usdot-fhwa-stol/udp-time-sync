#include <gtest/gtest.h>
#include <TimeSync.hpp>
#include <udp-socket/UdpClient.hpp>
#include <chrono>

TEST(TimeSync, now) {
    setenv("SIMULATION_MODE", "FALSE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    time_sync.start();  
    EXPECT_NEAR(time_sync.nowInMilliseconds(), 
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(),
        100); 

}

TEST(TimeSync, readTimeSyncMessage) {

    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    auto timeSyncMessage = carma_streets_time_sync::readTimeSyncMessage(time_sync_message);
    EXPECT_EQ(timeSyncMessage.timestamp, 1231);
    EXPECT_EQ(timeSyncMessage.timestep, 1000);
}

TEST(TimeSync, readTimeSyncMessageInvalid) {

    std::string time_sync_message = "{\"something\": 1231, \"timestep\": 1000}";
    EXPECT_THROW( carma_streets_time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
    time_sync_message = "{\"timestamp\": 1231, \"something\": 1000}";
    EXPECT_THROW( carma_streets_time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
    time_sync_message = "invalidJson";
    EXPECT_THROW( carma_streets_time_sync::readTimeSyncMessage(time_sync_message), std::runtime_error);
}

TEST(TimeSync, testStart) {
    setenv("SIMULATION_MODE", "TRUE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    time_sync.start();  
    udp_socket::UdpClient client("127.0.0.1", 4567);
    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    client.Send(time_sync_message);

    EXPECT_EQ(time_sync.nowInMilliseconds(), 1231); 
    EXPECT_NO_THROW(time_sync.sleepUntil(1231));
}
