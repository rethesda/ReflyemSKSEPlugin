#pragma once

#include "Config.h"

namespace Reflyem {
namespace SpeedCasting {
auto on_update_actor(RE::Character& player, float, const Reflyem::Config& config) -> void;
}
} // namespace Reflyem