#include <gtest/gtest.h>
#include <TimeSync.hpp>
#include <chrono>

TEST(TimeSync, now) {
    setenv("SIMULATION_MODE", "FALSE", 1);

    carma_streets_time_sync::TimeSync time_sync;
    time_sync.start();  
    sleep(1);
    EXPECT_EQ(time_sync.now(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); 
}
