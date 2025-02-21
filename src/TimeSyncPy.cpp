#include <pybind11/pybind11.h>
#include <string>
#include "TimeSync.hpp"
namespace py = pybind11;

PYBIND11_MODULE(libudp_time_sync, m)
{
    m.doc() = R"(This C++ library is meant to help integrate services into our [CDA Sim](https://github.com/usdot-fhwa-stol/cdasim)
         environment by allowing services to replace direct calls to system time with calls to our 
         [CARMA Clock](https://github.com/usdot-fhwa-stol/carma-time-lib) time abstration. When an environment variable called 
         `SIMULATION_MODE` is set to `TRUE`, this library will start a detached thread responsible for listening on a configurable 
         port of incoming UDP Time Synchronization messages that will allow services in the simulation environment to renaming synchronized 
         to dynamic timestep durations.)";

    py::class_<carma_streets_time_sync::TimeSync>(m, "TimeSync")
        .def(py::init<std::string, unsigned int>(), py::arg("ip") = "127.0.0.1", py::arg("port") = 4567,
            R"(Constructor for TimeSync object)")
        .def("nowInMilliseconds", &carma_streets_time_sync::TimeSync::nowInMilliseconds ,
            R"(Get current time in milliseconds)")
        .def("start", &carma_streets_time_sync::TimeSync::start, 
            R"(If in simulation mode, starts independent thread to consume time sync messages and update carma-clock)")
        .def("sleepUntil", &carma_streets_time_sync::TimeSync::sleepUntil, py::arg("future_time"),
            R"(Method will block thread until given time (ms))")
        .def("sleep_for", &carma_streets_time_sync::TimeSync::sleep, py::arg("time_to_sleep"),
            R"(Method will block thread for given time (ms))");
}
