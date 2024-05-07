#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H
#include "ultratypes.h"

/**
 * Debug Camera
*/

typedef union {
    struct {
        /* 0x00 */ s8 continueFlag;
        /* 0x01 */ s8 cameraRoll;
        /* 0x02 */ u16 nextPointFrame;
        /* 0x04 */ f32 viewAngle; // in degrees
        /* 0x08 */ Vec3s pos;
    };
    s32 _words[4];
} CutsceneCameraPoint; // size = 0x10

#define CS_CAM_CONTINUE 0

#define CS_CAM_DATA_NOT_APPLIED 0xFFFF

typedef struct {
    /* 0x0000 */ s16 mode;
    /* 0x0002 */ s16 nFrames;
    /* 0x0004 */ s16 nPoints;
    /* 0x0006 */ s16 unkIdx;
    /* 0x0008 */ s16 unk_08;
    /* 0x000A */ s16 unk_0A;
    /* 0x000C */ s32 unk_0C; // bool: indicates position vs lookAt?
    /* 0x0010 */ char unk_10[0x14];
    /* 0x0024 */ CutsceneCameraPoint position[129];
    /* 0x0834 */ CutsceneCameraPoint lookAt[129];
    /* 0x1044 */ s16 demoCtrlMenu;
    /* 0x1046 */ s16 demoCtrlActionIdx; // e (?), s (save), l (load), c (clear)
    /* 0x1048 */ s16 demoCtrlToggleSwitch;
    /* 0x104A */ Vec3s unk_104A;
} DebugCamSub; // size = 0x1050

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ Vec3f at;
    /* 0x10 */ Vec3f eye;
    /* 0x1C */ Vec3f unk_1C;
    /* 0x28 */ char unk_28[0xC];
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 unk_3C; // bool
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ f32 fov;
    /* 0x4C */ s16 roll;
    /* 0x4E */ char unk_4E[0x2];
    /* 0x50 */ f32 rollDegrees;
    /* 0x54 */ Vec3f unk_54;
    /* 0x60 */ Vec3f unk_60;
    /* 0x6C */ Vec3f unk_6C;
    /* 0x78 */ s16 unk_78;
    /* 0x7A */ s16 unk_7A;
    /* 0x7C */ DebugCamSub sub;
} DebugCam; // size = 0x10CC

typedef struct {
    /* 0x00 */ char letter;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s16 mode;
    /* 0x04 */ CutsceneCameraPoint* position;
    /* 0x08 */ CutsceneCameraPoint* lookAt;
    /* 0x0C */ s16 nFrames;
    /* 0x0E */ s16 nPoints;
} DebugCamCut; // size = 0x10

typedef struct {
    /* 0x00 */ f32 curFrame;
    /* 0x04 */ f32 unk_04; // frame count?
    /* 0x08 */ s16 keyframe;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x10 */ Vec3f positionPos; // confusing name
    /* 0x1C */ Vec3f lookAtPos;
    /* 0x28 */ f32 roll;
    /* 0x2C */ f32 fov;
} DebugCamAnim; // size = 0x30

typedef enum {
    /* 0 */ DEBUG_CAM_TEXT_YELLOW,
    /* 1 */ DEBUG_CAM_TEXT_PEACH,
    /* 2 */ DEBUG_CAM_TEXT_BROWN,
    /* 3 */ DEBUG_CAM_TEXT_ORANGE,
    /* 4 */ DEBUG_CAM_TEXT_GOLD,
    /* 5 */ DEBUG_CAM_TEXT_WHITE,
    /* 6 */ DEBUG_CAM_TEXT_BLUE,
    /* 7 */ DEBUG_CAM_TEXT_GREEN
} DebugCamTextColor;

// Converts a file letter to its numerical index
#define MEMPAK_LETTER_TO_INDEX(c) ((c) - 'A')

// Converts a numerical index to a file letter
#define MEMPAK_INDEX_TO_LETTER(i) ((i) + 'A')

#endif // DEBUG_UTILS_H