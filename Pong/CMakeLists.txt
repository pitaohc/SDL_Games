# 设置最小版本
cmake_minimum_required(VERSION 3.4.1)
# 设置cpp版本为17
set(CMAKE_CXX_STANDARD 17)

project(Pong LANGUAGES CXX)


find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Game.h Game.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
