cmake_minimum_required(VERSION 3.20)
project(cpp_demos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# 自动收集所有 demos下的 cpp文件
file(GLOB_RECURSE DEMO_SOURCES
        CONFIGURE_DEPENDS
        demos/*.cpp)

foreach(src ${DEMO_SOURCES})
    get_filename_component(name ${src} NAME_WE)
    get_filename_component(dir  ${src} DIRECTORY)

    # 防止同名 cpp 冲突（用路径生成唯一名字）
    file(RELATIVE_PATH rel ${CMAKE_SOURCE_DIR} ${src})
    string(REPLACE "/" "_" target ${rel})
    string(REPLACE "." "_" target ${target})

    add_executable(${target} ${src})
endforeach()
