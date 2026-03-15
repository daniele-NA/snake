cmake_minimum_required(VERSION 3.22.1)
project("cherry")

add_library(cherry SHARED main.cpp)

find_package(game-activity REQUIRED CONFIG)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

target_link_libraries(cherry
    game-activity::game-activity_static
    EGL
    GLESv3
    jnigraphics
    android
    log)
