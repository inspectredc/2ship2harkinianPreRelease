#ifndef ENHANCEMENTS_H
#define ENHANCEMENTS_H

#include "Camera/DebugCam.h"
#include "Camera/FreeLook.h"
#include "Camera/Rotate.h"
#include "Cheats/MoonJump.h"
#include "Cheats/Infinite.h"
#include "Cheats/UnbreakableRazorSword.h"
#include "Cycle/EndOfCycle.h"
#include "Masks/FierceDeityAnywhere.h"
#include "Masks/NoBlastMaskCooldown.h"
#include "Masks/FastTransformation.h"
#include "Minigames/AlwaysWinDoggyRace.h"
#include "Cutscenes/SkipEntranceCutscenes.h"
#include "Cutscenes/HideTitleCards.h"
#include "Restorations/PowerCrouchStab.h"
#include "Restorations/SideRoll.h"
#include "Graphics/PlayAsKafei.h"
#include "Songs/EnableSunsSong.h"

enum AlwaysWinDoggyRaceOptions {
    ALWAYS_WIN_DOGGY_RACE_OFF,
    ALWAYS_WIN_DOGGY_RACE_MASKOFTRUTH,
    ALWAYS_WIN_DOGGY_RACE_ALWAYS,
};

#ifdef __cplusplus
extern "C" {
#endif

void InitEnhancements();

#ifdef __cplusplus
}
#endif

#endif // ENHANCEMENTS_H
