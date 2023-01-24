#pragma once

#include "Config.hpp"

namespace Reflyem {
namespace Core {

template <typename L, typename R> struct Either {
  bool isL;
  union {
    L left;
    R right;
  };
  Either(const L& l) {
    isL  = true;
    left = l;
  }
  Either(const R& r) {
    isL   = false;
    right = r;
  }
};

static std::map<std::uintptr_t, ULONGLONG> character_timer_map100;
static std::map<std::uintptr_t, ULONGLONG> character_timer_map1000;

auto character_timer_map_handler(ULONGLONG now_time, std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void;

auto get_random_int() -> int;

auto damage_actor_value(RE::Actor& actor, RE::ActorValue av, float value) -> void;

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void;

auto can_modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float a_value, RE::ActorValue av)
    -> bool;

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

auto get_effects_magnitude_sum(std::vector<RE::ActiveEffect*>& effects) -> std::optional<float>;

auto get_effects_by_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> std::vector<RE::ActiveEffect*>;

auto get_dual_value_mult(RE::ValueModifierEffect& value_effect) -> float;

auto get_second_av(RE::ValueModifierEffect& value_effect) -> RE::ActorValue;

auto getting_damage_mult(RE::Actor& actor) -> float;

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void;

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster) -> void;

auto is_power_attacking(RE::Actor& actor) -> bool;

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

} // namespace Core
} // namespace Reflyem