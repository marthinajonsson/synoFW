cmake_minimum_required(VERSION 3.12)
project(synoFW)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenSSL)

include_directories(inc)

set(Includes
        #${CMAKE_CURRENT_LIST_DIR}/inc/Cli.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/Methods.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/RequestHandler.h inc/ErrorCodes.h inc/FileStationAPI.h inc/Utilities.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RequestHandler.cpp src/FileStationAPI.cpp)


add_executable(synoFW ${Sources} ${Includes})


target_link_libraries(synoFW curl)
target_link_libraries(synoFW jsoncpp)
target_link_libraries(synoFW /usr/share/boost-build)
