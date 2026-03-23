#pragma once

#include <jni.h>
#include <android/log.h>
#include <time.h>

#define LOG_TAG "MY-LOG"
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define OPEN_GL_DIVIDER 255

// == use 'static' to avoid pointer escaping , this approach isn't THREAD-SAFE == //
float *opengl_color(float r, float g, float b, float a) {
    static float array[4];
    array[0] = r / OPEN_GL_DIVIDER;
    array[1] = g / OPEN_GL_DIVIDER;
    array[2] = b / OPEN_GL_DIVIDER;
    array[3] = a / OPEN_GL_DIVIDER;

    LOG_I("Color RGBA(%.0f, %.0f, %.0f, %.0f) -> GL(%.3f, %.3f, %.3f, %.3f)", r, g, b, a, array[0], array[1], array[2], array[3]);
    return array;
}

static double get_time_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}