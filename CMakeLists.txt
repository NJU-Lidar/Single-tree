cmake_minimum_required(VERSION 3.10)
project(SingleTreeSegmentation VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译选项
if(MSVC)
    add_compile_options(/W3 /EHsc)
    # 禁用一些MSVC的警告
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 如果是Debug模式，添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/Zi)
    else()
        add_compile_options(-g)
    endif()
endif()

# 源文件
set(SOURCES
    Single-tree/Single-tree.cpp
)

# 可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置输出名称
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "single_tree_segmentation"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 安装数据文件（可选）
install(DIRECTORY Single-tree/data/
    DESTINATION share/${PROJECT_NAME}/data
    FILES_MATCHING PATTERN "*.txt"
)

# 安装文档
install(FILES README.md LICENSE
    DESTINATION share/${PROJECT_NAME}
)

# 打印配置信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

