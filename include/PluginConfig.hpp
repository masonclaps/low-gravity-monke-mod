  
#pragma once

#include "main.hpp"

#include <string>
#include <unordered_map>

//#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(PluginConfig,

CONFIG_VALUE(isTrigger, bool, "IsTrigger", false);

CONFIG_VALUE(gravityAmount, float, "GravityAmount", 0.5f);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(isTrigger);
        CONFIG_INIT_VALUE(gravityAmount);
    )
)