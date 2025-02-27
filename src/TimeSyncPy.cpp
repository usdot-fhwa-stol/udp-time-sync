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
    
    m.def("nowInMilliseconds", &time_sync::nowInMilliseconds ,
            R"(Get current time in milliseconds)");
    m.def("sleepUntil", &time_sync::sleepUntil, py::arg("future_time"),
        R"(Block thread until given time (ms))");
    m.def("sleep_for", &time_sync::sleep, py::arg("time_to_sleep"),
        R"(Block thread for given time (ms))");
    py::class_<time_sync::TimeSync>(m, "TimeSync")
        .def(py::init<std::string, unsigned int>(), py::arg("ip") = "127.0.0.1", py::arg("port") = 4567,
            R"(Constructor for TimeSync object)")
        .def("start", &time_sync::TimeSync::start, 
            R"(If in simulation mode, starts independent thread to consume time sync messages and update carma-clock)")
        .def("stop", &time_sync::TimeSync::stop, 
            R"(If in simulation mode, stops independent thread to consume time sync messages and update carma-clock)");
        ;
}
