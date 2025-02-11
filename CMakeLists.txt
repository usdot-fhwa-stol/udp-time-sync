cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "/opt/carma/cmake/")

# Required for std::lock and syntax
set(CMAKE_CXX_STANDARD 17)



project(carma_streets_time_sync
    DESCRIPTION "CARMA Streets Time Synchronization Library"
    HOMEPAGE_URL https://github.com/usdot-fhwa-stol/carma-streets-time-sync
	VERSION 0.0.1
    LANGUAGES CXX
)


option(BUILD_TESTS "Build tests" OFF)
option(CREATE_DEB_PACKAGE "Create Debian Package" OFF)

include(cmake/dependencies.cmake)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
add_definitions(-DRAPIDJSON_HAS_STDSTRING=1)


add_library(${PROJECT_NAME} SHARED )

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME} PUBLIC    
    ::kafka-client 
    ::carma-clock 
    rapidjson 
    spdlog
    Python3::Python
    pybind11::module
)
# RPATH required for allowing dpkg-shlibdeps to find stol-apt-repo dependencies
set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
)

include(CommonSource)
if (BUILD_TESTS)
    include(Testing)
    include(CodeCoverage)
endif()
include(InstallingGeneral)
include(InstallingConfigs)

if (CREATE_DEB_PACKAGE)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "kafka-client (>= 1.0), carma-clock (>= 1.0)")
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "")

    include(Packing)
endif()



