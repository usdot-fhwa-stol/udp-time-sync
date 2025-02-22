find_package(udp-socket REQUIRED)
find_package(carma-clock REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(spdlog REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include(GetCPM)
set(CPM_USE_LOCAL_PACKAGES ON)
CPMAddPackage(
    NAME pybind11
    GITHUB_REPOSITORY pybind/pybind11
    SYSTEM ON
    GIT_TAG v2.12.0
    OPTIONS
        "PYBIND11_INSTALL ON"
        "PYBIND11_TEST OFF"
)
