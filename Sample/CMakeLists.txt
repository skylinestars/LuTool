# Define target name
set (TARGET_NAME Sample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project( ${TARGET_NAME} )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ADD_EXECUTABLE(${TARGET_NAME} Main.cpp)

target_include_directories( ${TARGET_NAME} PUBLIC 
				${CMAKE_SOURCE_DIR}/Source)

target_link_libraries( ${TARGET_NAME} PUBLIC LuTool)


