cmake_minimum_required(VERSION 3.12)
project(synoFW)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenSSL)

include_directories(inc)

set(Includes
        #${CMAKE_CURRENT_LIST_DIR}/inc/Cli.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Methods.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/RequestHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ErrorCodes.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/FileStationAPI.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Utilities.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/HttpRequests.h inc/API.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RequestHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FileStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/VideoStationAPI.cpp)


add_executable(synoFW ${Sources} ${Includes})


target_link_libraries(synoFW curl)
target_link_libraries(synoFW jsoncpp)
target_link_libraries(synoFW /usr/share/boost-build)
