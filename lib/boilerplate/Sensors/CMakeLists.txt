cmake_minimum_required(VERSION 2.8.3)
project(rti_ros)

MESSAGE("BUILDING LIBRARIES IN:")
MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Sensors
)

file(GLOB SOURCE_Sensors
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Sensors/Sensors.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Sensors/*Plugin.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Sensors/*Support.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Sensors/*Boilerplate.cpp
)

