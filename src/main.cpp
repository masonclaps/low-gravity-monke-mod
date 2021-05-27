#include <thread>
#include "main.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "Utils/FileUtils.hpp"
#include "audiocliploader.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

#include "GlobalNamespace/GorillaTagManager.hpp"
MAKE_HOOK_OFFSETLESS(GorillaTagManager_Start, void, GlobalNamespace::GorillaTagManager* self) {
    GorillaTagManager_Start(self);
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    std::string songDir = "/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MP3PlayerMod/Songs/";
    
    FileUtils::mkdir(songDir);

    INSTALL_HOOK_OFFSETLESS(getLogger(), GorillaTagManager_Start, il2cpp_utils::FindMethodUnsafe("", "GorillaTagManager", "Awake", 0));
    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    getLogger().info("Installed all hooks!");

}