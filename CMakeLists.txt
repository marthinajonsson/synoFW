cmake_minimum_required(VERSION 3.12)
project(synoFW)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenSSL)

include_directories(inc)
include_directories(ifc)

set(Includes
        #${CMAKE_CURRENT_LIST_DIR}/inc/Cli.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Methods.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/RequestHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ErrorCodes.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FileStationAPI.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Utilities.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/HttpRequests.h inc/API.h inc/ParamHandling.h inc/RequestUrlBuilder.h inc/FilenameStructure.h inc/ImdbStructure.h ifc/Observer.h ifc/Subject.h ifc/Logger.h inc/StatusLogger.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RequestHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FileStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/VideoStationAPI.cpp src/ImdbStructure.cpp)


add_executable(synoFW ${Sources} ${Includes})


target_link_libraries(synoFW curl)
target_link_libraries(synoFW jsoncpp)
target_link_libraries(synoFW /usr/share/boost-build)
