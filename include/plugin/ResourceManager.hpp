#pragma once

#include "AnimationEventHandler.hpp"
#include "Config.hpp"

namespace Reflyem {
namespace ResourceManager {

using FormMask  = std::array<std::array<std::int16_t, 3>, 1>;
using ActorMask = std::array<std::array<std::int16_t, 3>, 3>;

struct DrainValues {
public:
  float stamina;
  float health;
  float magicka;

  DrainValues(float a_stamina, float a_health, float a_magicka);

  auto drain(RE::Actor& actor) -> void;
};

auto calc_mask_sum(FormMask& f_mask) -> std::int32_t;

auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost) -> std::shared_ptr<DrainValues>;

auto ranged_spend_handler() -> void;

auto update_actor(RE::Character& character, float delta, const Reflyem::Config& config) -> void;

auto animation_handler(Reflyem::AnimationEventHandler::AnimationEvent animation, RE::Actor& actor, bool is_power_attack,
                       const Reflyem::Config config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Reflyem::Config& config) -> void;

} // namespace ResourceManager
} // namespace Reflyem