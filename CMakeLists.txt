cmake_minimum_required(VERSION 3.0)
project(server)

#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} -g)

#配置最终生成的可执行文件路径存储
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#配置头文件的搜索
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/include/server/DB)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
#加载字目录
add_subdirectory(src)