cmake_minimum_required(VERSION 3.14)
project(synoFW)

find_package(OpenSSL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 COMPONENTS thread chrono filesystem)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(inc)
include_directories(ifc)
include_directories(unittest)

include_directories(external/gtest/googlemock)
add_subdirectory(external/gtest/googlemock)

set(Test
        ${CMAKE_CURRENT_LIST_DIR}/unittest/TestStartup.h
        ${CMAKE_CURRENT_LIST_DIR}/unittest/CacheTest.h
        ${CMAKE_CURRENT_LIST_DIR}/unittest/FsRequestTest.h
        ${CMAKE_CURRENT_LIST_DIR}/unittest/VsRequestTest.h
        ${CMAKE_CURRENT_LIST_DIR}/unittest/FsResponseTest.h
        ${CMAKE_CURRENT_LIST_DIR}/unittest/FileStationApiTest.h unittest/MockAPI.h)

set(Includes
        ${CMAKE_CURRENT_LIST_DIR}/inc/Methods.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/RequestHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ErrorCodes.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FileStationAPI.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Utilities.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/HttpRequests.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/API.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ParamHandling.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/RequestUrlBuilder.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FilenameStructure.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/CacheMgr.h
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Observer.h
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Subject.h
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Logger.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/EventLogger.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FileLogger.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/DatabaseObject.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/JsonStreamer.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ActiveObject.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Queue.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Dispatcher.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FileMgr.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ConfigHelper.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/TablePrinter.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Imdb.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RequestHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FileStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/VideoStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CacheMgr.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Subject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/JsonStreamer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ParamHandling.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/API.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TablePrinter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Imdb.cpp)



enable_testing()

add_executable(synoFW ${Sources} ${Includes} ${Test})


target_link_libraries(synoFW curl)
target_link_libraries(synoFW gmock)
target_link_libraries(synoFW ${Boost_LIBRARIES})