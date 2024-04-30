#include "2s2h/SaveManager/SaveManager.h"
#include "z64.h"

// To better support future migrations, we always store the `Save` at the `save` key to be consistent across normal saves and owl saves
void SaveManager_Migration_2(nlohmann::json& j) {
    bool isOwlSave = j.contains("save");

    if (!isOwlSave) {
        nlohmann::json saveContext;
        saveContext["save"] = j;

        j = saveContext;
    }

    nlohmann::json dpadEquips;
    for (int i = 0; i < EQUIP_SLOT_D_MAX; i++) {
        dpadEquips["dpadItems"][i] = ITEM_NONE;
        dpadEquips["dpadSlots"][i] = SLOT_NONE;
    }

    j["save"]["additionalSaveInfo"]["dpadEquips"] = dpadEquips;
}