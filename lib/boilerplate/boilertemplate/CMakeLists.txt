cmake_minimum_required(VERSION 2.8.3)
project(rti_ros)

MESSAGE("BUILDING LIBRARIES IN:")
MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Template
)

file(GLOB SOURCE_Template
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Template/Template.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Template/*Plugin.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Template/*Support.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/boilerplate/Template/*Boilerplate.cpp
)

