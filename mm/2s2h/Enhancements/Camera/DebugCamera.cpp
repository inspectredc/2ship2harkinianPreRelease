#include "DebugCamera.h"
#include <libultraship/bridge.h>
#include "Enhancements/GameInteractor/GameInteractor.h"
#include "CameraUtils.h"
#include "DebugUtils.h"

extern "C" {
#include <variables.h>
#include <z64camera.h>
extern CameraSetting sCameraSettings[];
extern void DebugCamera_Enable(DebugCam* debugCam, Camera* cam);
extern void DebugCamera_Update(DebugCam* debugCam, Camera* cam);
}

extern DebugCam debugCam;

static uint32_t debugCameraHookId = 0;

void RegisterDebugCamera() {
    if (debugCameraHookId) {
        GameInteractor::Instance->UnregisterGameHook<GameInteractor::ShouldVanillaBehavior>(debugCameraHookId);
        debugCameraHookId = 0;
        gDbgCamEnabled = false;
    }

    if (CVarGetInteger("gEnhancements.Camera.DebugCamera", 0)) {
        debugCameraHookId = REGISTER_VB_SHOULD(GI_VB_USE_CUSTOM_CAMERA, {
                Camera* camera = static_cast<Camera*>(opt);
                if (!gDbgCamEnabled) {
                    gDbgCamEnabled = true;
                    DebugCamera_Enable(&debugCam, camera);
                }

                if (gDbgCamEnabled) {
                    DebugCamera_Update(&debugCam, camera);
                    *should = false;
                }
        });
    }
}
