#include "Enhancements.h"

void InitEnhancements() {
    // Camera
    RegisterCameraFreeLook();
    RegisterDebugCamera();

    // Cheats
    RegisterInfiniteCheats();
    RegisterMoonJumpOnL();

    // Cycle
    RegisterEndOfCycleSaveHooks();

    // Masks
    RegisterFastTransformation();
    RegisterFierceDeityAnywhere();
    RegisterNoBlastMaskCooldown();

    // Restorations
    RegisterSideRoll();
    
    // Cutscenes
    RegisterSkipEntranceCutscenes();
    RegisterHideTitleCards();
    
    // Modes
    RegisterPlayAsKafei();

}
