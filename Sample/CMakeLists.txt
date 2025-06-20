# Define target name
set (TARGET_NAME Sample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project( ${TARGET_NAME} )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ADD_EXECUTABLE(${TARGET_NAME} test_logger.cpp)#test_logger.cpp

target_include_directories( ${TARGET_NAME} PUBLIC 
				${CMAKE_SOURCE_DIR}/Source)

#target_link_libraries( ${TARGET_NAME} PUBLIC LuTool)

#LuTool
set(LUTOOL_LIB_RELEASE "D:/Soft/LuTool/lib/release/LuTool.lib")
set(LUTOOL_LIB_DEBUG "D:/Soft/LuTool/lib/debug/LuTool.lib")
set(LUTOOL_DLL_RELEASE "D:/Soft/LuTool/bin/release/LuTool.dll")
set(LUTOOL_DLL_DEBUG "D:/Soft/LuTool/bin/debug/LuTool.dll")
set(LUTOOL_INCLUDE "D:/Soft/LuTool/include")
set(HAS_FMT_LIB)
target_include_directories(${TARGET_NAME} PUBLIC 
  ${LUTOOL_INCLUDE})
target_link_libraries(${TARGET_NAME} PUBLIC ${LUTOOL_LIB_DEBUG})

set(FMT_DIR "D:/Soft/FMT")
if(EXISTS "${FMT_DIR}")
    message("Found fmt library at: ${FMT_DIR}")
    set(fmt_DIR "${FMT_DIR}/lib/cmake/fmt")
    find_package(fmt REQUIRED)
    add_definitions(-DFMT_FOUND)
    target_link_libraries(${TARGET_NAME} PUBLIC fmt::fmt)
else()
    message("fmt library not found in custom paths.")
endif()