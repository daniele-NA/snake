cmake_minimum_required(VERSION 3.22.1)
project("snake")

add_library(snake SHARED
    main.cpp
    renderer/renderer.c
    app/game/snake.c
    )

find_package(game-activity REQUIRED CONFIG)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

target_link_libraries(snake
    game-activity::game-activity_static
    EGL
    GLESv3
    jnigraphics
    android
    log)
