#include <libultraship/bridge.h>
#include "Enhancements/GameInteractor/GameInteractor.h"
#include "CameraUtils.h"

extern "C" {
#include <z64.h>
#include <z64camera.h>
#include <macros.h>
#include <functions.h>
extern CameraSetting sCameraSettings[];
// extern s32 sCamerainterfaceFlags;
extern f32 Camera_GetFocalActorHeight(Camera* camera);
extern f32 Camera_ScaledStepToCeilF(f32 target, f32 cur, f32 stepScale, f32 minDiff);
extern s16 Camera_GetPitchAdjFromFloorHeightDiffs(Camera* camera, s16 viewYaw, s16 shouldInit);
extern s16 Camera_ScaledStepToFloorS(s16 target, s16 cur, f32 stepScale, s16 minDiff);
extern f32 Camera_CalcSlopeYAdj(Vec3f* floorNorm, s16 playerYRot, s16 eyeAtYaw, f32 adjAmt);
extern void Camera_ScaledStepToCeilVec3f(Vec3f* target, Vec3f* cur, f32 xzStepScale, f32 yStepScale, f32 minDiff);
extern s32 Camera_BgCheckInfo(Camera* camera, Vec3f* from, CameraCollision* to);
extern s32 func_800CBC84(Camera* camera, Vec3f* from, CameraCollision* to, s32 arg3);
extern f32 Camera_ClampLerpScale(Camera* camera, f32 maxLerpScale);
}

bool OotFunc_800458D4(Camera* camera, VecGeo* eyeAtDir, f32 yOffset, f32* arg3, s16 calcSlopeYAdj) {
    f32 phi_f2;
    Vec3f playerToAtOffsetTarget;
    Vec3f atTarget;
    f32 eyeAtAngle;
    PosRot* playerPosRot = &camera->focalActorPosRot;
    f32 deltaY;
    Vec3f* at = &camera->at;
    f32 temp;

    playerToAtOffsetTarget.y = Camera_GetFocalActorHeight(camera) + yOffset;
    playerToAtOffsetTarget.x = 0.0f;
    playerToAtOffsetTarget.z = 0.0f;

    if (calcSlopeYAdj) {
        playerToAtOffsetTarget.y -=
            Camera_CalcSlopeYAdj(&camera->floorNorm, playerPosRot->rot.y, eyeAtDir->yaw, CAM_SLOPE_Y_ADJ_AMOUNT);
    }

    deltaY = playerPosRot->pos.y - *arg3;
    eyeAtAngle = Math_FAtan2F(deltaY, OLib_Vec3fDistXZ(at, &camera->eye));

    if (eyeAtAngle > DEG_TO_RAD(CAM_GLOBAL_32)) {
        phi_f2 = 1.0f - sinf(eyeAtAngle - DEG_TO_RAD(CAM_GLOBAL_32));
    } else if (eyeAtAngle < DEG_TO_RAD(CAM_GLOBAL_33)) {
        phi_f2 = 1.0f - sinf(DEG_TO_RAD(CAM_GLOBAL_33) - eyeAtAngle);
    } else {
        phi_f2 = 1.0f;
    }

    playerToAtOffsetTarget.y -= deltaY * phi_f2;
    Camera_ScaledStepToCeilVec3f(&playerToAtOffsetTarget, &camera->focalActorAtOffset, CAM_GLOBAL_29, CAM_GLOBAL_30, 0.1f);

    atTarget.x = playerPosRot->pos.x + camera->focalActorAtOffset.x;
    atTarget.y = playerPosRot->pos.y + camera->focalActorAtOffset.y;
    atTarget.z = playerPosRot->pos.z + camera->focalActorAtOffset.z;

    temp = camera->atLerpStepScale;
    Camera_ScaledStepToCeilVec3f(&atTarget, at, temp, camera->atLerpStepScale, 0.2f);

    return true;
}

bool OotCamera_CalcAtForParallel(Camera* camera, VecGeo* arg1, f32 yOffset, f32* arg3, s16 calcSlopeYAdj) {
    Vec3f* at = &camera->at;
    Vec3f playerToAtOffsetTarget;
    Vec3f atTarget;
    Vec3f* eye = &camera->eye;
    PosRot* playerPosRot = &camera->focalActorPosRot;
    f32 temp_f2;
    f32 phi_f16;
    f32 eyeAtDistXZ;
    f32 phi_f20;
    f32 playerHeight = Camera_GetFocalActorHeight(camera);

    playerToAtOffsetTarget.x = 0.0f;
    playerToAtOffsetTarget.y = playerHeight + yOffset;
    playerToAtOffsetTarget.z = 0.0f;

    if (PREG(76) && calcSlopeYAdj) {
        playerToAtOffsetTarget.y -=
            Camera_CalcSlopeYAdj(&camera->floorNorm, playerPosRot->rot.y, arg1->yaw, CAM_SLOPE_Y_ADJ_AMOUNT);
    }

    if (camera->focalActorFloorHeight == camera->focalActorPosRot.pos.y || ((Player*)camera->focalActor)->actor.gravity > -0.1f ||
        ((Player*)camera->focalActor)->stateFlags1 & PLAYER_STATE1_1000) {
        *arg3 = Camera_ScaledStepToCeilF(playerPosRot->pos.y, *arg3, CAM_GLOBAL_43, 0.1f);
        phi_f20 = playerPosRot->pos.y - *arg3;
        playerToAtOffsetTarget.y -= phi_f20;
        Camera_ScaledStepToCeilVec3f(&playerToAtOffsetTarget, &camera->focalActorAtOffset, camera->yOffsetUpdateRate,
                             camera->xzOffsetUpdateRate, 0.1f);
    } else {
        if (!PREG(75)) {
            phi_f20 = playerPosRot->pos.y - *arg3;
            eyeAtDistXZ = OLib_Vec3fDistXZ(at, &camera->eye);
            phi_f16 = eyeAtDistXZ;
            Math_FAtan2F(phi_f20, eyeAtDistXZ);
            temp_f2 = Math_FTanF(DEG_TO_RAD(camera->fov * 0.4f)) * phi_f16;
            if (temp_f2 < phi_f20) {
                *arg3 += phi_f20 - temp_f2;
                phi_f20 = temp_f2;
            } else if (phi_f20 < -temp_f2) {
                *arg3 += phi_f20 + temp_f2;
                phi_f20 = -temp_f2;
            }
            playerToAtOffsetTarget.y -= phi_f20;
        } else {
            phi_f20 = playerPosRot->pos.y - *arg3;
            temp_f2 = Math_FAtan2F(phi_f20, OLib_Vec3fDistXZ(at, eye));
            if (DEG_TO_RAD(CAM_GLOBAL_32) < temp_f2) {
                phi_f16 = 1 - sinf(temp_f2 - DEG_TO_RAD(CAM_GLOBAL_32));
            } else if (temp_f2 < DEG_TO_RAD(CAM_GLOBAL_33)) {
                phi_f16 = 1 - sinf(DEG_TO_RAD(CAM_GLOBAL_33) - temp_f2);
            } else {
                phi_f16 = 1;
            }
            playerToAtOffsetTarget.y -= phi_f20 * phi_f16;
        }
        Camera_ScaledStepToCeilVec3f(&playerToAtOffsetTarget, &camera->focalActorAtOffset, CAM_GLOBAL_29, CAM_GLOBAL_30, 0.1f);
        camera->yOffsetUpdateRate = CAM_GLOBAL_29;
        camera->xzOffsetUpdateRate = CAM_GLOBAL_30;
    }
    atTarget.x = playerPosRot->pos.x + camera->focalActorAtOffset.x;
    atTarget.y = playerPosRot->pos.y + camera->focalActorAtOffset.y;
    atTarget.z = playerPosRot->pos.z + camera->focalActorAtOffset.z;
    Camera_ScaledStepToCeilVec3f(&atTarget, at, camera->atLerpStepScale, camera->atLerpStepScale, 0.2f);
    return true;
}

bool OotCamera_Parallel1(Camera* camera) {
    Vec3f* eye = &camera->eye;
    Vec3f* at = &camera->at;
    Vec3f* eyeNext = &camera->eyeNext;
    f32 spB8;
    f32 spB4;
    s16 tangle;
    VecGeo spA8;
    VecGeo atToEyeDir;
    VecGeo atToEyeNextDir;
    PosRot* playerPosRot = &camera->focalActorPosRot;
    CameraCollision cameraCollision;
    s16 sp6A;
    s16 phi_a0;
    Parallel1ReadOnlyData* roData = &camera->paramData.para1.roData;
    Parallel1ReadWriteData* rwData = &camera->paramData.para1.rwData;
    f32 pad2;
    f32 playerHeight;
    s32 pad3;

    playerHeight = Camera_GetFocalActorHeight(camera);
    if (RELOAD_PARAMS(camera)) {
        CameraModeValue* values = sCameraSettings[camera->setting].cameraModes[camera->mode].values;
        f32 yNormal = 1.0f + CAM_YOFFSET_NORM - (CAM_YOFFSET_NORM * (68.0f / playerHeight));

        roData->unk_00 = GET_NEXT_SCALED_RO_DATA(values) * playerHeight * yNormal;
        roData->unk_04 = GET_NEXT_SCALED_RO_DATA(values) * playerHeight * yNormal;
        roData->unk_20 = CAM_DEG_TO_BINANG(GET_NEXT_RO_DATA(values));
        roData->unk_22 = CAM_DEG_TO_BINANG(GET_NEXT_RO_DATA(values));
        roData->unk_0C = GET_NEXT_RO_DATA(values);
        roData->unk_10 = GET_NEXT_RO_DATA(values);
        roData->unk_14 = GET_NEXT_RO_DATA(values);
        roData->unk_18 = GET_NEXT_SCALED_RO_DATA(values);
        roData->interfaceFlags = GET_NEXT_RO_DATA(values);
        roData->unk_08 = GET_NEXT_SCALED_RO_DATA(values) * playerHeight * yNormal;
        roData->unk_1C = GET_NEXT_SCALED_RO_DATA(values);
    }

    atToEyeDir = OLib_Vec3fDiffToVecGeo(at, eye);
    atToEyeNextDir = OLib_Vec3fDiffToVecGeo(at, eyeNext);

    switch (camera->animState) {
        case 0:
        case 10:
        case 20:
        case 25:
            // rwData->unk_16 = 0;
            rwData->unk_1C = 0;
            if (roData->interfaceFlags & PARALLEL1_FLAG_2) {
                rwData->timer2 = 20;
            } else {
                rwData->timer2 = CAM_DEFAULT_ANIM_TIME;
            }
            rwData->unk_10.x = 0.0f;
            rwData->unk_04 = playerPosRot->pos.y - camera->unk_0F0.y;
            camera->animState++;
            break;
    }

    if (rwData->timer3 != 0) {
        if (roData->interfaceFlags & PARALLEL1_FLAG_1) {
            // Rotate roData->unk_22 degrees from behind the player.
            rwData->unk_1E = (s16)(playerPosRot->rot.y - 0x7FFF) + roData->unk_22;
        } else if (roData->interfaceFlags & PARALLEL1_FLAG_2) {
            // rotate to roData->unk_22
            rwData->unk_1E = roData->unk_22;
        } else {
            // leave the rotation alone.
            rwData->unk_1E = atToEyeNextDir.yaw;
        }
    } else {
        if (roData->interfaceFlags & PARALLEL1_FLAG_5) {
            rwData->unk_1E = (s16)(playerPosRot->rot.y - 0x7FFF) + roData->unk_22;
        }
        // todo: figure out why external linkage isn't working
        // sCamerainterfaceFlags = roData->interfaceFlags;
    }

    rwData->unk_20 = roData->unk_20;

    if (camera->animState == 21) {
        // rwData->unk_16 = 1;
        camera->animState = 1;
    } else if (camera->animState == 11) {
        camera->animState = 1;
    }

    spB8 = CAM_UPDATE_RATE_STEP_SCALE_XZ * camera->speedRatio;
    spB4 = CAM_UPDATE_RATE_STEP_SCALE_Y * camera->speedRatio;

    camera->rUpdateRateInv = Camera_ScaledStepToCeilF(CAM_R_UPDATE_RATE_INV, camera->rUpdateRateInv, spB8, 0.1f);
    camera->yawUpdateRateInv = Camera_ScaledStepToCeilF(roData->unk_0C, camera->yawUpdateRateInv, spB8, 0.1f);
    camera->pitchUpdateRateInv = Camera_ScaledStepToCeilF(2.0f, camera->pitchUpdateRateInv, spB4, 0.1f);
    camera->xzOffsetUpdateRate = Camera_ScaledStepToCeilF(CAM_XZ_OFFSET_UPDATE_RATE, camera->xzOffsetUpdateRate, spB8, 0.1f);
    camera->yOffsetUpdateRate = Camera_ScaledStepToCeilF(CAM_Y_OFFSET_UPDATE_RATE, camera->yOffsetUpdateRate, spB4, 0.1f);
    camera->fovUpdateRate =
        Camera_ScaledStepToCeilF(CAM_FOV_UPDATE_RATE, camera->fovUpdateRate, camera->speedRatio * 0.05f, 0.1f);

    if (roData->interfaceFlags & PARALLEL1_FLAG_0) {
        tangle = Camera_GetPitchAdjFromFloorHeightDiffs(camera, atToEyeDir.yaw - 0x7FFF, true);

        spB8 = ((1.0f / roData->unk_10) * 0.3f);
        pad2 = (((1.0f / roData->unk_10) * 0.7f) * (1.0f - camera->speedRatio));
        rwData->unk_1C = Camera_ScaledStepToFloorS(tangle, rwData->unk_1C, spB8 + pad2, 0xF);
    } else {
        rwData->unk_1C = 0;
    }

    if (camera->focalActorFloorHeight == camera->focalActorPosRot.pos.y || ((Player*)camera->focalActor)->actor.gravity > -0.1f ||
        ((Player*)camera->focalActor)->stateFlags1 & PLAYER_STATE1_1000) {
        rwData->unk_04 = playerPosRot->pos.y;
        sp6A = false;
    } else {
        sp6A = true;
    }

    if (!(roData->interfaceFlags & PARALLEL1_FLAG_7) && !sp6A) {
        OotCamera_CalcAtForParallel(camera, &atToEyeNextDir, roData->unk_00, &rwData->unk_04,
                                 roData->interfaceFlags & PARALLEL1_FLAG_0);
    } else {
        OotFunc_800458D4(camera, &atToEyeNextDir, roData->unk_18, &rwData->unk_04,
                      roData->interfaceFlags & PARALLEL1_FLAG_0);
    }

    if (rwData->timer3 != 0) {
        Camera_SetStateFlag(camera, CAM_STATE_DISABLE_MODE_CHANGE);
        tangle = (((rwData->timer3 + 1) * rwData->timer3) >> 1);
        spA8.yaw = atToEyeDir.yaw + (((s16)(rwData->unk_1E - atToEyeDir.yaw) / tangle) * rwData->timer3);
        spA8.pitch = atToEyeDir.pitch;
        spA8.r = atToEyeDir.r;
        rwData->timer3--;
    } else {
        // rwData->unk_16 = 0;
        camera->dist = Camera_ScaledStepToCeilF(roData->unk_04, camera->dist, 1.0f / camera->rUpdateRateInv, 2.0f);
        spA8 = OLib_Vec3fDiffToVecGeo(at, eyeNext);
        spA8.r = camera->dist;

        if (roData->interfaceFlags & PARALLEL1_FLAG_6) {
            spA8.yaw = Camera_ScaledStepToFloorS(rwData->unk_1E, atToEyeNextDir.yaw, 0.6f, 0xA);
        } else {
            spA8.yaw = Camera_ScaledStepToFloorS(rwData->unk_1E, atToEyeNextDir.yaw, 0.8f, 0xA);
        }

        if (roData->interfaceFlags & PARALLEL1_FLAG_0) {
            phi_a0 = rwData->unk_20 - rwData->unk_1C;
        } else {
            phi_a0 = rwData->unk_20;
        }

        spA8.pitch = Camera_ScaledStepToFloorS(phi_a0, atToEyeNextDir.pitch, 1.0f / camera->pitchUpdateRateInv, 4);

        if (spA8.pitch > DEG_TO_BINANG(79.655f)) {
            spA8.pitch = DEG_TO_BINANG(79.655f);
        }

        if (spA8.pitch < DEG_TO_BINANG(29.995f)) {
            spA8.pitch = DEG_TO_BINANG(29.995f);
        }
    }
    *eyeNext = OLib_AddVecGeoToVec3f(at, &spA8);
    if (camera->status == CAM_STATUS_ACTIVE) {
        cameraCollision.pos = *eyeNext;
        if (!camera->play->envCtx.skyboxDisabled || roData->interfaceFlags & PARALLEL1_FLAG_4) {
            Camera_BgCheckInfo(camera, at, &cameraCollision);
            *eye = cameraCollision.pos;
        } else {
            func_800CBC84(camera, at, &cameraCollision, 0);
            *eye = cameraCollision.pos;
            spA8 = OLib_Vec3fDiffToVecGeo(eye, at);
            camera->inputDir.x = spA8.pitch;
            camera->inputDir.y = spA8.yaw;
            camera->inputDir.z = 0;
        }
    }
    camera->fov = Camera_ScaledStepToCeilF(roData->unk_14, camera->fov, camera->fovUpdateRate, 1.0f);
    camera->roll = Camera_ScaledStepToFloorS(0, camera->roll, 0.5, 0xA);
    camera->atLerpStepScale = Camera_ClampLerpScale(camera, sp6A ? roData->unk_1C : roData->unk_18);
    return true;
}

void RegisterOotCamera() {
    REGISTER_VB_SHOULD(GI_VB_USE_CUSTOM_CAMERA, {
        if (CVarGetInteger("gEnhancements.Camera.OotCamera", 0)) {
            Camera* camera = static_cast<Camera*>(opt);
            // Todo: Fill out more cases
            switch (sCameraSettings[camera->setting].cameraModes[camera->mode].funcId) {
                case CAM_FUNC_PARALLEL1:
                    OotCamera_Parallel1(camera);
                    *should = false;
                    break;
                default:
                    break;
            }
        }
    });
}
