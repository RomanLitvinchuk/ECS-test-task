cmake_minimum_required(VERSION 3.15)
project(ECS-test-task LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(SOURCES
    ECS.cpp
    scene.cpp
    systems.cpp
    scene.h
    components.h
    systems.h
)  

add_executable(ECS ${SOURCES})
target_link_libraries(ECS PRIVATE nlohmann_json::nlohmann_json)