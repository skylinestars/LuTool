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
                     ${CMAKE_CURRENT_SOURCE_DIR}   
                     ${CMAKE_SOURCE_DIR}/Thirdparty/lz4_1.10/include
)

# 链接库
target_link_libraries(${PROJECT_NAME} PUBLIC 
                     ${CMAKE_SOURCE_DIR}/Thirdparty/lz4_1.10/lib/liblz4.lib)



set(FMT_DIR "D:/Soft/FMT")
if(EXISTS "${FMT_DIR}")
    message("Found fmt library at: ${FMT_DIR}")
    set(fmt_DIR "${FMT_DIR}/lib/cmake/fmt")
    find_package(fmt REQUIRED)
    add_definitions(-DFMT_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
else()
    message("fmt library not found in custom paths.")
endif()

  
# 设置输出目录（可选）  
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")  
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")  
  
# 安装库文件  
install(TARGETS ${PROJECT_NAME}  
    LIBRARY DESTINATION lib  # 安装到 ${CMAKE_INSTALL_PREFIX}/lib  
    ARCHIVE DESTINATION lib  # 对于静态库，也安装到 ${CMAKE_INSTALL_PREFIX}/lib  
    RUNTIME DESTINATION bin  # 对于可执行文件（如果有的话），安装到 ${CMAKE_INSTALL_PREFIX}/bin  
)  
  
# 安装头文件  
install(DIRECTORY ${SOURCE_DIR}  
    DESTINATION include/LuTool # 安装到 ${CMAKE_INSTALL_PREFIX}/include，保持原有目录结构  
    FILES_MATCHING PATTERN "*.h"  # 只安装 .h 文件 
)
