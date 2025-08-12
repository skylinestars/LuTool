set(PROJECT_NAME LuTool)
# 配置目录结构
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/Source")

group_sources("${SOURCE_DIR}" "${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE source_file "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE header_file "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
# 添加动态库目标  
add_library(${PROJECT_NAME} SHARED ${source_file} ${header_file})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/LuTool/Source>
)

# 链接库
target_link_libraries(${PROJECT_NAME} PUBLIC 
                     fmt::fmt)
  
# 设置输出目录（可选）  
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")  
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")  
  
# 安装库文件 - 支持多配置
install(TARGETS ${PROJECT_NAME}  
    EXPORT LuToolTargets
    LIBRARY DESTINATION lib  # 安装到 ${CMAKE_INSTALL_PREFIX}/lib  
    ARCHIVE DESTINATION lib  # 对于静态库，也安装到 ${CMAKE_INSTALL_PREFIX}/lib  
    RUNTIME DESTINATION bin  # 对于可执行文件（如果有的话），安装到 ${CMAKE_INSTALL_PREFIX}/bin  
    CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel
)  
  
# 安装头文件  
install(DIRECTORY ${SOURCE_DIR}  
    DESTINATION include/LuTool # 安装到 ${CMAKE_INSTALL_PREFIX}/include，保持原有目录结构  
    FILES_MATCHING PATTERN "*.h"  # 只安装 .h 文件 
)

# 安装导出目标
install(EXPORT LuToolTargets
    FILE LuToolTargets.cmake
    NAMESPACE LuTool::
    DESTINATION lib/cmake/LuTool
)

# 创建Config文件
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LuToolConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

# 设置Config文件需要的变量
set(LuTool_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/LuTool/Source")

# 创建Config.cmake文件
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/CMake/LuToolConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LuToolConfig.cmake"
    INSTALL_DESTINATION lib/cmake/LuTool
    PATH_VARS LuTool_INCLUDE_DIRS
)

# 安装Config文件
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LuToolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LuToolConfigVersion.cmake"
    DESTINATION lib/cmake/LuTool
)
