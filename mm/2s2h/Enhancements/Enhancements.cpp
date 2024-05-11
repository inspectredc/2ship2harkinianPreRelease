#include "Enhancements.h"

void InitEnhancements() {
    // Camera
    RegisterCameraFreeLook();
    RegisterCameraRotate();
    RegisterDebugCam();

    // Cheats
    RegisterInfiniteCheats();
    RegisterMoonJumpOnL();

    // Cycle
    RegisterEndOfCycleSaveHooks();

    // Masks
    RegisterFastTransformation();
    RegisterFierceDeityAnywhere();
    RegisterNoBlastMaskCooldown();

    // Minigames
    RegisterAlwaysWinDoggyRace();

    // Songs
    RegisterEnableSunsSong();
    
    // Restorations
    RegisterPowerCrouchStab();
    RegisterSideRoll();
    
    // Cutscenes
    RegisterSkipEntranceCutscenes();
    RegisterHideTitleCards();
    
    // Modes
    RegisterPlayAsKafei();

}
