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
        case APP_CMD_KEY_EVENT:
            LOG_I("key event");
            break;
        case APP_CMD_TOUCH_EVENT:
            LOG_I("touched");
            break;
        case APP_CMD_RESUME:
            LOG_I("App resumed");
            char ln[3] = {0};
            AConfiguration_getLanguage(pt_app->config, ln);
            int orientation = AConfiguration_getOrientation((*pt_app).config);
            AConfiguration_setOrientation((*pt_app).config, 2);
            LOG_I("SDK %d, Language %s, Orientation %d", pt_app->activity->sdkVersion, ln, orientation);
            break;
    }
}

#ifdef __cplusplus
}
#endif
