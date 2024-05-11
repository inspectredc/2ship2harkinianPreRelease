#include "Rotate.h"
#include <libultraship/bridge.h>
#include "Enhancements/GameInteractor/GameInteractor.h"
#include "CameraUtils.h"

static uint32_t rotateCamVBHookId = 0;

void RegisterCameraRotate() {
    static f32 sCamSpin = 0.0f;

    if (rotateCamVBHookId) {
        GameInteractor::Instance->UnregisterGameHook<GameInteractor::AfterCameraUpdate>(rotateCamVBHookId);
        rotateCamVBHookId = 0;
    }

    if (CVarGetInteger("gEnhancements.Camera.Spin", 0)) {
        rotateCamVBHookId = GameInteractor::Instance->RegisterGameHook<GameInteractor::AfterCameraUpdate>([](Camera* camera) {
            camera->roll = DEG_TO_BINANG(sCamSpin);
            sCamSpin += 2.0f;
            if (sCamSpin > 180.0f) {
                sCamSpin -= 360.0f;
            }
        });
    }
}