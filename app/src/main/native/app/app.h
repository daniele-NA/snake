#pragma once

#include <stdint.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "../utils/utils.h"
#include "../renderer/renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void app_callback(struct android_app *pt_app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            renderer_init(pt_app->window);
            break;
        case APP_CMD_TERM_WINDOW:
            renderer_destroy();
            break;
        case APP_CMD_CONFIG_CHANGED:
            LOG_I("Config changed");
            break;
        default:
            break;
    }
}

#ifdef __cplusplus
}
#endif
