cmake_minimum_required(VERSION 3.12)
project(synoFW)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenSSL)

include_directories(inc)
include_directories(ifc)

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
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Imdb.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ImdbBasics.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ImdbAkas.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ImdbCrew.h
        ${CMAKE_CURRENT_LIST_DIR}/inc/ImdbName.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RequestHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FileStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/VideoStationAPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CacheMgr.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ifc/Subject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/JsonStreamer.cpp)


add_executable(synoFW ${Sources} ${Includes})


target_link_libraries(synoFW curl)
target_link_libraries(synoFW jsoncpp)
target_link_libraries(synoFW /usr/share/boost-build)
