#include <pybind11/pybind11.h>
#include <string>
#include "TimeSync.hpp"
namespace py = pybind11;

PYBIND11_MODULE(libcarma_streets_time_sync, m)
{
    m.doc() = R"(CARMA Time Module provides the CarmaClock object, which is a 
        wrapper for direct calls to system time. It is intended for use when it is 
        necessary to have control over each increment in time. An example of a use 
        case is in a simulation environment where time may progress non-linearly. 
        The CarmaClock constructor takes a boolean parameter to indicate whether 
        to directly make calls to system time or to use its internal store value
        for time. Calls to its update() method will update the internal stored value,
        while calls to its now* or sleep* will return or unblock threads depending 
        on the stored value respectively.)";

    py::class_<carma_streets_time_sync::TimeSync>(m, "TimeSync")
        .def(py::init<>())
        .def("nowInMilliseconds", &carma_streets_time_sync::TimeSync::nowInMilliseconds ,
            R"(Get current time in milliseconds)")
        .def("start", &carma_streets_time_sync::TimeSync::start, 
            R"(Starts independent thread to consume time sync messages)")
        .def("sleep_until", &carma_streets_time_sync::TimeSync::sleep_until, py::arg("future_time"),
            R"(Method will block thread until given time (ms))")
        .def("sleep_for", &carma_streets_time_sync::TimeSync::sleep, py::arg("time_to_sleep"),
            R"(Method will block thread for given time (ms))");
}
