cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(iot_high_perf)
find_package(Drogon CONFIG REQUIRED)
add_executable(iot_server main.cc)
target_link_libraries(iot_server PRIVATE Drogon::Drogon)