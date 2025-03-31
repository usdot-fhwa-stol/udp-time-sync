# UDP Time Sync
This C++ library is meant to help integrate services into our [CDA Sim](https://github.com/usdot-fhwa-stol/cdasim) environment by allowing services to replace direct calls to system time with calls to our [CARMA Clock](https://github.com/usdot-fhwa-stol/carma-time-lib) time abstration. When an environment variable called `SIMULATION_MODE` is set to `TRUE`, this library will start a detached thread responsible for listening on a configurable port of incoming UDP Time Synchronization messages that will allow services in the simulation environment to synchronize to dynamic timestep durations.

## CI Status

These badges are for the default branch only.

[![Build Workflow](https://github.com/usdot-fhwa-stol/udp-time-sync/actions/workflows/build.yml/badge.svg)](https://github.com/usdot-fhwa-stol/udp-time-sync/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=usdot-fhwa-stol_udp-time-sync&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=usdot-fhwa-stol_udp-time-sync)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=usdot-fhwa-stol_udp-time-sync&metric=coverage)](https://sonarcloud.io/summary/new_code?id=usdot-fhwa-stol_udp-time-sync)

## Using the Library

The following sections will describe how to use the UDP Time Sync Library in your C++ or Python application

### Install library

The easiest way to include the UDP Time Sync Library in your application is to install it via `apt`.
**Prerequisites**

- Ubuntu ( jammy, focal,)
- CMake 3.10
- Python 3
- [rapidjson](https://github.com/Tencent/rapidjson)
- [carma-clock](https://github.com/usdot-fhwa-stol/carma-time-lib)
- [udp-socket](https://github.com/usdot-fhwa-stol/udp-socket)

The UDP Time Sync library is one the the FHWA (Federal Highway Administration) STOL (Saxton Transportation Operation Library) libraries built into a Debian package via CMake/CPack scripts from the [carma-builds](https://github.com/usdot-fhwa-stol/carma-builds) repository. CI (Continuous Integration) scripts also push this Debian package to a STOL Debian Package repository. To install this package you must only add this repository to `apt`.

```shell
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
. /etc/lsb-release
# add the STOL APT repository
echo "deb [trusted=yes] http://s3.amazonaws.com/stol-apt-repository develop ${DISTRIB_CODENAME} " > /etc/apt/sources.list.d/stol-apt-repository.list
apt update
apt install carma-clock-1 udp-socket-1 udp-time-sync-1
```

This steps above add the relevent STOL apt repository for pulling correct debian package.

To add to you CMake Project simply
```CMake
list(APPEND CMAKE_PREFIX_PATH "/opt/carma/cmake/")

find_package(udp-time-sync REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC ::udp-time-sync)

```
To include in python simply 
```python
import importlib
libTimeSync = importlib.import_module("libudp_time_sync")
```
> [!IMPORTANT]  
> The Python Path needs to include a path to the installed .so file.
```
export PYTHONPATH="$PYTHONPATH:/opt/carma/lib"
```
### Configuration
Several environment variables allow you to configure the functionality of this library. 
`SIMULATION_MODE` is an environment variable that when set to `TRUE` launches a thread to listen for time sync messages and update the underlying carma clock object. When set to false the carma clock object simply functions as regular system time.
`PERFORMANCE_LOGGING` is an environment variable that creates a file logger that will log real time vs simulation time in a CSV file to evaluate time sychronization
`LOGS_DIR` is an enviroment variable that allows you to specify where this log file will be written to.

## Contribution

Welcome to the CARMA contributing guide. Please read this guide to learn about our development process, how to propose pull requests and improvements, and how to build and test your changes to this project. [CARMA Contributing Guide](https://github.com/usdot-fhwa-stol/carma-platform/blob/develop/Contributing.md)

## Code of Conduct

Please read our [CARMA Code of Conduct](https://github.com/usdot-fhwa-stol/carma-platform/blob/develop/Code_of_Conduct.md) which outlines our expectations for participants within the CARMA community, as well as steps to reporting unacceptable behavior. We are committed to providing a welcoming and inspiring community for all and expect our code of conduct to be honored. Anyone who violates this code of conduct may be banned from the community.

## Attribution

The development team would like to acknowledge the people who have made direct contributions to the design and code in this repository. [CARMA Attribution](https://github.com/usdot-fhwa-stol/carma-platform/blob/develop/ATTRIBUTION.md)

## License

By contributing to the Federal Highway Administration (FHWA) Connected Automated Research Mobility Applications (CARMA), you agree that your contributions will be licensed under its Apache License 2.0 license. [CARMA License](https://github.com/usdot-fhwa-stol/carma-platform/blob/develop/docs/License.md)

## Contact

Please click on the CARMA logo below to visit the Federal Highway Adminstration(FHWA) CARMA website.

[![CARMA Image](https://raw.githubusercontent.com/usdot-fhwa-stol/CARMAPlatform/develop/docs/image/CARMA_icon.png)](https://highways.dot.gov/research/research-programs/operations/CARMA)

