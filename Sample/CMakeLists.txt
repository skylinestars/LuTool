# Define target name
set (TARGET_NAME Sample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project( ${TARGET_NAME} )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ADD_EXECUTABLE(${TARGET_NAME} test_logger.cpp)

# 使用find_package方式依赖LuTool
# 设置LuTool的安装路径，让CMake能找到LuToolConfig.cmake
set(CMAKE_PREFIX_PATH "D:/Soft/LuToolRel;${CMAKE_PREFIX_PATH}")

# 查找LuTool包
find_package(LuTool REQUIRED)

# 链接LuTool库 - 这会自动处理所有依赖（包括FMT）
target_link_libraries(${TARGET_NAME} PUBLIC LuTool::LuTool)

# 包含LuTool的头文件目录
target_include_directories(${TARGET_NAME} PUBLIC ${LuTool_INCLUDE_DIRS})

message("LuTool found: ${LuTool_FOUND}")
message("LuTool version: ${LuTool_VERSION}")
message("LuTool build type: ${LuTool_BUILD_TYPE}")
