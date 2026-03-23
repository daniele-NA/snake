#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#include <android/log.h>
#include <math.h>
#include <time.h>
#include "utils/utils.h"
#include "app/app.h"
#include "app/game/snake.h"

static bool motion_event_filter_func(const GameActivityMotionEvent *pt_event) {
    int source_class = pt_event->source & AINPUT_SOURCE_CLASS_MASK;
    return (source_class == AINPUT_SOURCE_CLASS_POINTER ||
            source_class == AINPUT_SOURCE_CLASS_JOYSTICK);
}

/*
 * OpenGL NDC (Normalized Device Coordinates):
 *
 *       +1.0
 *         |
 *         |
 * -1.0 ---+--- +1.0
 *         |
 *         |
 *       -1.0
 *
 * Triangles are the building block of everything in OpenGL -
 * any shape (squares, circles, 3D models) is made of triangles.
 * The GPU is optimized to fill them.
 *
 * Shaders are compiled at runtime and sent to the GPU.
 */


// global variables

static s_game game;
static bool game_started = false;
static float touch_down_x = 0 , touch_down_y = 0 ;

extern "C" void android_main(struct android_app *pt_app) {
    pt_app->onAppCmd = app_callback;
    android_app_set_motion_event_filter(pt_app, motion_event_filter_func);

    while (!pt_app->destroyRequested) {
        int events;
        struct android_poll_source *pt_source;
        int result = ALooper_pollOnce(0, NULL, &events, (void **) &pt_source);

        switch (result) {
            case ALOOPER_POLL_TIMEOUT:
            case ALOOPER_POLL_WAKE:
                break;
            case ALOOPER_EVENT_ERROR:
                LOG_E("ALooper_pollOnce error");
                break;
            default:
                if (pt_source) {
                    pt_source->process(pt_app, pt_source);
                }
        }

        android_input_buffer *input = android_app_swap_input_buffers(pt_app);
        if (input) {
            for (int i = 0; i < input->motionEventsCount; i++) {
                GameActivityMotionEvent *event = &input->motionEvents[i];
                float x = GameActivityPointerAxes_getX(&event->pointers[0]);
                float y = GameActivityPointerAxes_getY(&event->pointers[0]);
                int action = event->action & AMOTION_EVENT_ACTION_MASK;

                // IT'S NOT SWIPE DOWN , IT'S WHEN OUR FINGER TAP THE SCREEN
                if (action == AMOTION_EVENT_ACTION_DOWN) {
                    touch_down_x = x;
                    touch_down_y = y;
                } else if (action == AMOTION_EVENT_ACTION_UP) {
                    if (game.state == GAME_OVER) {
                        game_restart(&game);
                    } else {
                        float dx = x - touch_down_x;
                        float dy = y - touch_down_y;
                        if (fabsf(dx) > fabsf(dy) && fabsf(dx) > 50) {
                            game_set_direction(&game, dx > 0 ? RIGHT : LEFT);
                        } else if (fabsf(dy) > fabsf(dx) && fabsf(dy) > 50) {
                            game_set_direction(&game, dy > 0 ? DOWN : UP);
                        }
                    }
                }
            }
            android_app_clear_motion_events(input);
            android_app_clear_key_events(input);
        }

        if (!game_started) {
            game_init(&game);

            // our local state of the game
            game_started = true;
        }
        game_update(&game, get_time_seconds());

        renderer_draw_game(&game);
    }
}
