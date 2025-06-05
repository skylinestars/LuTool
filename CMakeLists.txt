cmake_minimum_required(VERSION 3.15)
PROJECT (LuTool CXX)

set(CMAKE_CXX_STANDARD 11)  
set(CMAKE_CXX_STANDARD_REQUIRED True)  
option(LUTOOL_EXPORTS_OPTION "Enable export" ON)
option(SAMPLE "Enable Sample" ON)
if(LUTOOL_EXPORTS_OPTION)
	add_definitions(-DLUTOOL_EXPORTS_OPTION)
endif()

if(WIN32)
	set(CMAKE_INSTALL_PREFIX "D:/Soft/${PROJECT_NAME}")  

elseif(UNIX)
	set(CMAKE_INSTALL_PREFIX "/usr/local/${PROJECT_NAME}")  
endif()

message("Install presfix ${CMAKE_INSTALL_PREFIX}")

#CMAKE_BUILD_TYPE
set (CMAKE_BUILD_TYPE Debug CACHE STRING "build type(Debug,Release)")

include(CMake/Utils.cmake)
ADD_SUBDIRECTORY (Source)
if(SAMPLE)
	ADD_SUBDIRECTORY (Sample)
endif()
