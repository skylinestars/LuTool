set(PROJECT_NAME LuTool)
# 配置目录结构
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/Source")

group_sources("${SOURCE_DIR}" "${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE source_file "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE header_file "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
# 添加动态库目标  
add_library(${PROJECT_NAME} SHARED ${source_file} ${header_file})

target_include_directories(${PROJECT_NAME} PUBLIC
                     ${CMAKE_CURRENT_SOURCE_DIR}                  
)
  
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
    DESTINATION include  # 安装到 ${CMAKE_INSTALL_PREFIX}/include，保持原有目录结构  
    FILES_MATCHING PATTERN "*.h"  # 只安装 .h 文件 
)