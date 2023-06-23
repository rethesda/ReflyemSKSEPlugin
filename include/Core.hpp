#pragma once
#include <VersionHelpers.h>

namespace Reflyem::Core
{

enum ActorValueMain
{
  kHealth,
  kMagick,
  kStamina
};

template <typename L, typename R>
struct Either final
{
  bool is_l;

  union
  {
    L left;
    R right;
  };

  explicit Either(const L& l)
  {
    is_l = true;
    left = l;
  }

  explicit Either(const R& r)
  {
    is_l = false;
    right = r;
  }
};

struct ActorsCache final
{
  struct Data final
  {
    static constexpr auto POTIONS_ARRAY_SIZE = 128;
    float potions_cooldawn_timers[POTIONS_ARRAY_SIZE]{};

private:
    float last_delta_update_{0.f};

    float cast_on_crit_delay_{0.f};

    float timing_block_timer_{0.f};
    bool timing_block_flag_{false};
    std::int32_t timing_parry_counter_{0};
    float timing_parry_counter_timer_{0.f};

    float update_50_timer_{0.f};
    float update_100_timer_{0.f};
    float update_1000_timer_{0.f};

    float bash_parry_timer_{0.f};
    float bash_parry_timer_no_hit_{0.f};

    float petrified_blood_accumulator_{0.f};

    float absorb_shield_value_magick_{0.f};
    float absorb_shield_value_physic_{0.f};
    float absorb_shield_value_all_{0.f};

    float absorb_shield_from_attack_value_magick_{0.f};
    float absorb_shield_from_attack_value_physic_{0.f};
    float absorb_shield_from_attack_value_all_{0.f};

    uint64_t last_tick_count_{GetTickCount64()};

    static constexpr auto mod(auto& value, const auto delta) -> void
    {
      value += delta;
      if (value < 0) { value = 0; }
    }

    static constexpr auto set(auto& value, const auto set_value) -> void
    {
      if (set_value >= 0) { value = set_value; }
    }

public:
    // mods
    auto mod_timing_parry_counter(const int32_t count) -> void
    {
      mod(timing_parry_counter_, count);
    }

    auto mod_potions_cooldawn_timers(const float delta) -> void
    {
      for (int32_t index : views::iota(0, POTIONS_ARRAY_SIZE))
        {
          mod(potions_cooldawn_timers[index], delta);
        }
    }

    auto mod_timing_parry_counter_timer(const float delta) -> void
    {
      timing_parry_counter_timer_ += delta;
      if (timing_parry_counter_timer_ < 0)
        {
          timing_parry_counter(0);
          timing_parry_counter_timer_ = 0;
        }
    }

    auto mod_absorb_shield_value_magick(const float delta) -> void
    {
      mod(absorb_shield_value_magick_, delta);
    }
    auto mod_absorb_shield_value_physic(const float delta) -> void
    {
      mod(absorb_shield_value_physic_, delta);
    }
    auto mod_absorb_shield_value_all(const float delta) -> void
    {
      mod(absorb_shield_value_all_, delta);
    }
    auto mod_absorb_shield_from_attack_value_magick(const float delta) -> void
    {
      mod(absorb_shield_from_attack_value_magick_, delta);
    }
    auto mod_absorb_shield_from_attack_value_physic(const float delta) -> void
    {
      mod(absorb_shield_from_attack_value_physic_, delta);
    }
    auto mod_absorb_shield_from_attack_value_all(const float delta) -> void
    {
      mod(absorb_shield_from_attack_value_all_, delta);
    }

    auto mod_absorbs_not_from_attack(const float delta) -> void
    {
      mod_absorb_shield_value_all(delta);
      mod_absorb_shield_value_physic(delta);
      mod_absorb_shield_value_magick(delta);
    }

    auto mod_absorbs_from_attack(const float delta) -> void
    {
      mod_absorb_shield_from_attack_value_all(delta);
      mod_absorb_shield_from_attack_value_physic(delta);
      mod_absorb_shield_from_attack_value_magick(delta);
    }

    auto mod_all_absorbs(const float delta) -> void
    {
      mod_absorbs_not_from_attack(delta);
      mod_absorbs_from_attack(delta);
    }

    auto mod_cast_on_crit_delay(const float delta) -> void { mod(cast_on_crit_delay_, delta); }

    auto mod_timing_block_timer(const float delta) -> void { mod(timing_block_timer_, delta); }

    auto mod_update_50_timer(const float delta) -> void { mod(update_50_timer_, delta); }

    auto mod_update_100_timer(const float delta) -> void { mod(update_100_timer_, delta); }

    auto mod_update_1000_timer(const float delta) -> void { mod(update_1000_timer_, delta); }

    auto mod_bash_parry_timer(const float delta) -> void { mod(bash_parry_timer_, delta); }

    auto mod_bash_parry_timer_no_hit(const float delta) -> void
    {
      mod(bash_parry_timer_no_hit_, delta);
    }

    auto mod_petrified_blood_accumulator(const float delta) -> void
    {
      mod(petrified_blood_accumulator_, delta);
    }

    // property
    [[nodiscard]] std::int32_t timing_parry_counter() const { return timing_parry_counter_; }

    void timing_parry_counter(const std::int32_t timing_parry_counter)
    {
      set(timing_parry_counter_, timing_parry_counter);
    }

    [[nodiscard]] float timing_parry_counter_timer() const { return timing_parry_counter_timer_; }

    void timing_parry_counter_timer(const float timing_parry_counter_timer)
    {
      set(timing_parry_counter_timer_, timing_parry_counter_timer);
    }

    [[nodiscard]] float last_delta_update() const { return last_delta_update_; }

    void last_delta_update(const float last_delta_update)
    {
      set(last_delta_update_, last_delta_update);
    }

    [[nodiscard]] float cast_on_crit_delay() const { return cast_on_crit_delay_; }

    void cast_on_crit_delay(const float cast_on_crit_delay)
    {
      set(cast_on_crit_delay_, cast_on_crit_delay);
    }

    [[nodiscard]] float timing_block_timer() const { return timing_block_timer_; }

    void timing_block_timer(const float timing_block_timer)
    {
      if (!timing_block_flag_ && timing_block_timer_ <= 0.f)
        {
          timing_block_timer_ = timing_block_timer;
        }
    }

    [[nodiscard]] bool timing_block_flag() const { return timing_block_flag_; }

    void timing_block_flag(const bool timing_block_flag)
    {
      if (timing_block_flag) { timing_block_flag_ = true; }
      if (timing_block_timer() <= 0.f) { timing_block_flag_ = timing_block_flag; }
    }

    [[nodiscard]] float update_50_timer() const { return update_50_timer_; }

    void update_50_timer_refresh() { update_50_timer_ = 0.05f; }

    [[nodiscard]] float update_100_timer() const { return update_100_timer_; }

    void update_100_timer_refresh() { update_100_timer_ = 0.1f; }

    [[nodiscard]] float update_1000_timer() const { return update_1000_timer_; }

    void update_1000_timer_refresh() { update_1000_timer_ = 1.f; }

    [[nodiscard]] float bash_parry_timer() const { return bash_parry_timer_; }

    void bash_parry_timer(const float bash_parry_timer)
    {
      set(bash_parry_timer_, bash_parry_timer);
    }

    [[nodiscard]] float bash_parry_timer_no_hit() const { return bash_parry_timer_no_hit_; }

    void bash_parry_timer_no_hit(const float bash_parry_timer_no_hit)
    {
      set(bash_parry_timer_no_hit_, bash_parry_timer_no_hit);
    }

    [[nodiscard]] float petrified_blood_accumulator() const { return petrified_blood_accumulator_; }

    void petrified_blood_accumulator(const float petrified_blood_accumulator)
    {
      set(petrified_blood_accumulator_, petrified_blood_accumulator);
    }

    void absorb_shield_value_magick(const float absorb_shield_value_magick)
    {
      set(absorb_shield_value_magick_, absorb_shield_value_magick);
    }
    void absorb_shield_value_physic(const float absorb_shield_value_physic)
    {
      set(absorb_shield_value_physic_, absorb_shield_value_physic);
    }
    void absorb_shield_value_all(const float absorb_shield_value_all)
    {
      set(absorb_shield_value_all_, absorb_shield_value_all);
    }
    void absorb_shield_from_attack_value_magick(const float absorb_shield_from_attack_value_magick)
    {
      set(absorb_shield_from_attack_value_magick_, absorb_shield_from_attack_value_magick);
    }
    void absorb_shield_from_attack_value_physic(const float absorb_shield_from_attack_value_physic)
    {
      set(absorb_shield_from_attack_value_physic_, absorb_shield_from_attack_value_physic);
    }
    void absorb_shield_from_attack_value_all(const float absorb_shield_from_attack_value_all)
    {
      set(absorb_shield_from_attack_value_all_, absorb_shield_from_attack_value_all);
    }

    [[nodiscard]] float absorb_shield_value_magick() const { return absorb_shield_value_magick_; }
    [[nodiscard]] float absorb_shield_value_physic() const { return absorb_shield_value_physic_; }
    [[nodiscard]] float absorb_shield_value_all() const { return absorb_shield_value_all_; }
    [[nodiscard]] float absorb_shield_from_attack_value_magick() const
    {
      return absorb_shield_from_attack_value_magick_;
    }
    [[nodiscard]] float absorb_shield_from_attack_value_physic() const
    {
      return absorb_shield_from_attack_value_physic_;
    }
    [[nodiscard]] float absorb_shield_from_attack_value_all() const
    {
      return absorb_shield_from_attack_value_all_;
    }

    [[nodiscard]] uint64_t last_tick_count() const { return last_tick_count_; }

    void last_tick_count_refresh() { last_tick_count_ = GetTickCount64(); }

    // function
    auto update_handler(const float delta) -> void
    {
      const auto negative_delta = -std::abs(delta);
      mod_bash_parry_timer(negative_delta);
      mod_bash_parry_timer_no_hit(negative_delta);
      mod_cast_on_crit_delay(negative_delta);
      mod_timing_block_timer(negative_delta);
      mod_update_50_timer(negative_delta);
      mod_update_100_timer(negative_delta);
      mod_update_1000_timer(negative_delta);
      mod_timing_parry_counter_timer(negative_delta);
      mod_potions_cooldawn_timers(negative_delta);
    }
  };

  using FormsMap = std::unordered_map<RE::FormID, Data>;

  private:
  FormsMap cache_map_{};
  static constexpr uint64_t GARBAGE_TIME = 5000;
  std::mutex mutex_;
  static constexpr uint32_t LABEL = 'ACCA';
  // ReSharper disable once CppVariableCanBeMadeConstexpr
  static const int32_t SERIALIZATION_VERSION = 4;

  [[nodiscard]] static auto is_garbage(const Data& data) -> bool
  {
    return (GetTickCount64() - data.last_tick_count()) >= GARBAGE_TIME;
  }

  auto garbage_collector() -> void
  {
    if (!IsWindows8OrGreater()) { return; }
    for (const auto& [form_id, data] : cache_map_)
      {
        if (is_garbage(data)) { cache_map_.erase(form_id); }
      }
  }

  auto load(const SKSE::SerializationInterface& a_interface) -> void
  {
    const auto lock = std::lock_guard{mutex_};
    uint32_t type;
    uint32_t version;
    uint32_t length;

    cache_map_.clear();

    logger::info("Start read actors cache"sv);

    while (a_interface.GetNextRecordInfo(type, version, length))
      {
        switch (type)
          {
            case LABEL: {
              int32_t serialization_version;

              if (!a_interface.ReadRecordData(serialization_version))
                {
                  logger::error("Fail load ser version, return"sv);
                  cache_map_.clear();
                  return;
                }

              if (serialization_version != SERIALIZATION_VERSION)
                {
                  logger::warn(
                      "Serialization version mismatched, clear cache and return, "
                      "versions: {} | {}"sv,
                      serialization_version,
                      SERIALIZATION_VERSION);
                  cache_map_.clear();
                  return;
                }

              size_t size;

              if (!a_interface.ReadRecordData(size))
                {
                  logger::error("Fail load size"sv);
                  break;
                }

              for (size_t i = 0; i < size; ++i)
                {
                  RE::FormID form_id;
                  if (!a_interface.ReadRecordData(form_id))
                    {
                      logger::error("Fail read formid"sv);
                      break;
                    }

                  Data data;
                  if (!a_interface.ReadRecordData(data))
                    {
                      logger::error("Fail read formid"sv);
                      break;
                    }

                  logger::info(
                      "Success read record with FormId: {} Data last tick: {}"sv,
                      form_id,
                      data.last_tick_count());
                  cache_map_[form_id] = data;
                }
            }
            default: {
              logger::warn("Unrecognized signature type: {}"sv, type);
              break;
            }
          }
      }

    logger::info("Finish read actors cache"sv);
  }

  auto save(const SKSE::SerializationInterface& a_interface) -> void
  {
    const auto lock = std::lock_guard{mutex_};
    logger::info("Start write actors cache"sv);

    if (!a_interface.OpenRecord(LABEL, 1))
      {
        logger::error("Error when try open record REFL on save"sv);
        return;
      }
    garbage_collector();

    if (!a_interface.WriteRecordData(&SERIALIZATION_VERSION, sizeof SERIALIZATION_VERSION))
      {
        logger::error("Failed to write SERIALIZATION_VERSION"sv);
        return;
      }

    const size_t size = cache_map_.size();
    if (!a_interface.WriteRecordData(&size, sizeof size))
      {
        logger::error("Failed to write size of map"sv);
        return;
      }

    for (const auto& [form_id, data] : cache_map_)
      {
        if (!a_interface.WriteRecordData(&form_id, sizeof form_id))
          {
            logger::error("Failed to write form id"sv);
            return;
          }
        if (!a_interface.WriteRecordData(&data, sizeof data))
          {
            logger::error("Failed to write data"sv);
            return;
          }
      }
    logger::info("Finish write actors cache"sv);
  }

  public:
  static auto skse_save_callback(SKSE::SerializationInterface* a_interface) -> void
  {
    if (!a_interface)
      {
        logger::error("Null skse serialization interface error when save"sv);
        return;
      }
    get_singleton().save(*a_interface);
  }

  static auto skse_load_callback(SKSE::SerializationInterface* a_interface) -> void
  {
    if (!a_interface)
      {
        logger::error("Null skse serialization interface error when load"sv);
        return;
      }
    get_singleton().load(*a_interface);
  }

  auto at_try(const RE::FormID key) -> std::optional<std::reference_wrapper<Data>>
  {
    const auto it = cache_map_.find(key);

    if (it == cache_map_.end()) { return std::nullopt; }

    return it->second;
  }

  auto at(const RE::FormID key) -> std::reference_wrapper<Data> { return cache_map_.at(key); }

  [[nodiscard]] auto exist(const RE::FormID key) const -> bool { return cache_map_.contains(key); }

  auto get_or_add(const RE::FormID key) -> std::reference_wrapper<Data>
  {
    const auto lock = std::lock_guard{mutex_};
    if (exist(key))
      {
        const auto data = at(key);
        return data;
      }
    cache_map_[key] = Data();
    const auto data = at(key);
    return data;
  }

  static auto get_singleton() noexcept -> ActorsCache&
  {
    static auto cache = ActorsCache();

    return cache;
  }
};

auto character_timer_map_handler(
    ULONGLONG now_time,
    std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void;

auto bound_data_comparer(
    const RE::TESBoundObject::BOUND_DATA& bound_data,
    const int16_t comparer_value) -> bool;

auto get_random_int() -> int;

auto actor_from_ni_pointer(const RE::NiPointer<RE::TESObjectREFR>* ni_actor) -> RE::Actor*;

auto actor_from_actor_handle(const RE::ActorHandle* handle) -> RE::Actor*;

auto get_commander_actor(const RE::Actor* actor) -> RE::Actor*;

auto get_commanded_actors(const RE::Actor* actor) -> RE::BSTArray<RE::CommandedActorData>*;

auto is_player_ally(RE::Actor* actor) -> bool;

auto damage_actor_value(RE::Actor& actor, RE::ActorValue av, float value) -> void;

auto restore_actor_value(RE::Actor& actor, const RE::ActorValue av, const float value) -> void;

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void;

auto can_modify_actor_value(
    const RE::ValueModifierEffect* a_this,
    const RE::Actor* a_actor,
    float a_value,
    RE::ActorValue av) -> bool;

auto flash_hud_meter(const RE::ActorValue av) -> void;

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool;

auto try_actor_has_active_mgef_with_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword)
    -> bool;

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects)
    -> std::optional<float>;

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword)
    -> std::vector<RE::ActiveEffect*>;

auto get_dual_value_mult(const RE::ValueModifierEffect& active_effect) -> float;

auto get_second_av(const RE::ActiveEffect& active_effect) -> RE::ActorValue;

auto getting_damage_mult(RE::Actor& actor) -> float;

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void;

auto cast_on_handle_formlists(
    RE::BGSListForm* keywords,
    RE::BGSListForm* spells,
    RE::Actor& caster,
    RE::Actor& target) -> void;

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster)
    -> void;

auto is_power_attacking(RE::Actor& actor) -> bool;

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

auto try_has_absolute_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool;

auto is_casting_actor(RE::Character& character) -> bool;

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target)
    -> void;

auto place_at_me(
    RE::TESObjectREFR* target,
    RE::TESForm* form,
    std::uint32_t count,
    bool force_persist,
    bool initially_disabled) -> RE::TESObjectREFR*;

auto initialization_hit_data(
    RE::HitData& hit_data,
    RE::Actor* attacker,
    RE::Actor* target,
    RE::InventoryEntryData* weapon,
    bool is_left) -> void;

auto apply_all_combat_spells_from_attack(
    RE::Character* attacker,
    RE::TESObjectWEAP* weapon,
    bool is_left,
    RE::Actor* target) -> void;

auto get_poison(RE::InventoryEntryData* _this) -> RE::AlchemyItem*;

auto get_actor_value_max(RE::Actor* actor, const RE::ActorValue av) -> float;

auto get_weapon(const RE::Actor& actor, const bool is_left_hand, RE::TESObjectWEAP* fallback_weapon)
    -> RE::TESObjectWEAP*;

auto form_has_keyword(const RE::TESForm* form, const RE::BGSKeyword* keyword) -> bool;

auto is_dual_wielding(const RE::Actor* actor) -> bool;

auto is_bashing(const RE::Actor* attacker) -> bool;

auto play_sound(RE::BGSSoundDescriptorForm* sound, RE::Actor* actor) -> void;

} // namespace Reflyem::Core