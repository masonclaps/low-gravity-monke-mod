#include <thread>
#include "main.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/ForceMode.hpp"

#include "GorillaLocomotion/Player.hpp"

using namespace UnityEngine;
using namespace UnityEngine::XR;

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

float mass = 0;
float gravityScale = 0.25;
float globalGravity = -9.81;

#include "GlobalNamespace/GorillaTagManager.hpp"
MAKE_HOOK_OFFSETLESS(GorillaTagManager_Update, void, GlobalNamespace::GorillaTagManager* self) {
    using namespace GlobalNamespace;
    using namespace GorillaLocomotion;

    Player* playerInstance = Player::get_Instance();
    if(playerInstance == nullptr) return;

    Rigidbody* playerPhysics = playerInstance->playerRigidBody;
    if(playerPhysics == nullptr) return;

    bool rightInput = false;

    rightInput = OVRInput::Get(OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::RTouch);

    if(rightInput) {
        getLogger().info("TRUE");
        playerPhysics->set_useGravity(false);
        UnityEngine::Vector3 gravity = globalGravity * gravityScale * (UnityEngine::Vector3::get_up());
        playerPhysics->AddForce(gravity, UnityEngine::ForceMode::Acceleration);
    }
    else {
        getLogger().info("FALSE");
        playerPhysics->set_useGravity(true);
    }

    GorillaTagManager_Update(self);
}
MAKE_HOOK_OFFSETLESS(GorillaTagManager_Start, void, GlobalNamespace::GorillaTagManager* self) {
    using namespace GlobalNamespace;
    using namespace GorillaLocomotion;

    Player* playerInstance = Player::get_Instance();
    if(playerInstance == nullptr) return;

    Rigidbody* playerPhysics = playerInstance->playerRigidBody;
    if(playerPhysics == nullptr) return;
    
    mass = playerPhysics->get_mass();

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

    INSTALL_HOOK_OFFSETLESS(getLogger(), GorillaTagManager_Update, il2cpp_utils::FindMethodUnsafe("", "GorillaTagManager", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), GorillaTagManager_Start, il2cpp_utils::FindMethodUnsafe("", "GorillaTagManager", "Awake", 0));
    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    getLogger().info("Installed all hooks!");

}