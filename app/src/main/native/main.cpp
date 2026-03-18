#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#include <android/log.h>
#include "utils/utils.h"
#include "app/app.h"

static bool motion_event_filter_func(const GameActivityMotionEvent *pt_event) {
    int source_class = pt_event->source & AINPUT_SOURCE_CLASS_MASK;
    return (source_class == AINPUT_SOURCE_CLASS_POINTER ||
            source_class == AINPUT_SOURCE_CLASS_JOYSTICK);
}


/*
     OPEN-GL CANVAS

         +1.0
           |
           |
           |
  -1.0 ----+---- +1.0
           |
           |
           |
          -1.0

Sì. I triangoli sono la base di tutto in OpenGL —
   qualsiasi forma (quadrati, cerchi, modelli 3D) è fatta di triangoli. Il GPU è ottimizzato per riempirli.

 */
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

        renderer_draw();
    }
}
