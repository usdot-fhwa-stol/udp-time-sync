#include <gtest/gtest.h>
#include <TimeSync.hpp>
#include <udp-socket/UdpClient.hpp>
#include <chrono>

TEST(TimeSync, now) {
    setenv("SIMULATION_MODE", "FALSE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    time_sync.start();  
    sleep(1);
    EXPECT_EQ(time_sync.nowInMilliseconds(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); 

}

TEST(TimeSync, read_time_sync_message) {
    setenv("SIMULATION_MODE", "TRUE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    EXPECT_EQ(time_sync.read_time_sync_message(time_sync_message), 1231); 
}

TEST(TimeSync, testStart) {
    setenv("SIMULATION_MODE", "TRUE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    time_sync.start();  
    udp_socket::UdpClient client("127.0.0.1", 4567);
    std::string time_sync_message = "{\"timestamp\": 1231, \"timestep\": 1000}";
    client.Send(time_sync_message);
    sleep(1);

    EXPECT_EQ(time_sync.nowInMilliseconds(), 1231); 
    EXPECT_NO_THROW(time_sync.sleep_until(1231));
}
