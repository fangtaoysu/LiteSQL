cmake_minimum_required(VERSION 3.0.0)
project(LiteSQL)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

file (GLOB MAIN
    "src/main.cpp"
    "src/Database.cpp"
    "src/Table.cpp"
    )

add_executable(main ${MAIN})

# 链接需要的库
target_link_libraries(main stdc++fs)