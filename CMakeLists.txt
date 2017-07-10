cmake_minimum_required(VERSION 2.8.3)
project(rti_ros)

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(--std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(--std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# Make CMake aware of the cmake folder for local FindXXX scripts.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(RTI REQUIRED)

set(NDDSHOME $ENV{NDDSHOME})

include_directories(
  ${NDDSHOME}/include
  ${NDDSHOME}/include/ndds
  include
  lib/boilerplate/boilerplate
)

add_definitions(-DRTI_UNIX -ldl)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
  link_directories(${NDDSHOME}/lib/x64Linux3gcc4.8.2)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv7l")
  link_directories(${NDDSHOME}/lib/armv6vfphLinux3.xgcc4.7.2)
endif()

file(GLOB BOILERPLATE_SOURCE
  lib/boilerplate/boilerplate/Boilerplate.cpp
)

include(lib/boilerplate/Template/CMakeLists.txt)
include(lib/boilerplate/Sensors/CMakeLists.txt)

add_executable(main
  src/main.cpp
  ${BOILERPLATE_SOURCE}
  ${SOURCE_Template}
  ${SOURCE_Sensors}
)

target_link_libraries(main 
  nddsc    
  nddscpp
  nddscore
  ${CMAKE_DL_LIBS}  
)

add_executable(twin
  src/twin.cpp
  ${BOILERPLATE_SOURCE}
   ${SOURCE_Template}
  ${SOURCE_Sensors}
)

target_link_libraries(twin 
  nddsc    
  nddscpp
  nddscore
  ${CMAKE_DL_LIBS}  
)
