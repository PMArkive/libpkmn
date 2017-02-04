/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_gen2impl.hpp"
#include "database/index_to_string.hpp"

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/shininess.hpp>

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/time.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>

#define GEN2_PC_RCAST    reinterpret_cast<pksav_gen2_pc_pokemon_t*>(_native_pc)
#define GEN2_PARTY_RCAST reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

    pokemon_gen2impl::pokemon_gen2impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen2_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen2_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen2_pokemon_party_data_t));
        _our_party_mem = true;

        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = LIBPKMN_OT_NAME;

        // Set internal members
        GEN2_PC_RCAST->species = uint8_t(_database_entry.get_pokemon_index());

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(LIBPKMN_OT_ID & 0xFFFF));

        // TODO: Use PKSav PRNG after refactor merged in
        time_t now = 0;
        std::srand((unsigned int)std::time(&now));
        GEN2_PC_RCAST->ev_hp   = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_atk  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_def  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_spd  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_spcl = uint16_t(std::rand());

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_IVs_from_form(
                _database_entry.get_form()
            );
        } else {
            GEN2_PC_RCAST->iv_data = uint16_t(std::rand());
        }

        GEN2_PC_RCAST->friendship = uint8_t(_database_entry.get_base_friendship());

        set_level_met(level);
        set_location_met("Special", false);
        PKSAV_CALL(
            pksav_gen2_set_caught_data_time_field(
                &now,
                &GEN2_PC_RCAST->caught_data
            );
        )

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        set_level(level);
        _update_moves(-1);
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(pc->species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(party->pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::~pokemon_gen2impl() {
        if(_our_pc_mem) {
            delete GEN2_PC_RCAST;
        }
        if(_our_party_mem) {
            delete GEN2_PARTY_RCAST;
        }
    }

    void pokemon_gen2impl::set_form(
        const std::string &form
    ) {
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_IVs_from_form(form);
        }
    }

    std::string pokemon_gen2impl::get_nickname() {
        return _nickname;
    }

    void pokemon_gen2impl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        _nickname = nickname;
    }

    bool pokemon_gen2impl::is_shiny() {
        return pkmn::calculations::gen2_shiny(
                   _IVs["Attack"],
                   _IVs["Defense"],
                   _IVs["Speed"],
                   _IVs["Special"]
               );
    }

    void pokemon_gen2impl::set_shininess(
        bool value
    ) {
        if(value) {
            /*
             * This value sets all IVs to the maximum values that result in
             * a shiny Pokémon.
             */
            pokemon_scoped_lock lock(this);
            static const uint16_t shiny_iv_data = pksav_littleendian16(0xFAAA);
            GEN2_PC_RCAST->iv_data = shiny_iv_data;
            _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        } else {
            set_IV("Attack", 13);
        }

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    void pokemon_gen2impl::set_held_item(
        const std::string &held_item
    ) {
        // Make sure item is valid and is holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(not item.holdable()) {
            throw std::invalid_argument("This item is not holdable.");
        }

        pokemon_scoped_lock lock(this);

        GEN2_PC_RCAST->held_item = uint8_t(item.get_item_index());

        _update_held_item();
    }

    std::string pokemon_gen2impl::get_trainer_name() {
        return _trainer_name;
    }

    void pokemon_gen2impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name length must be 1-7."
                  );
        }

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen2impl::get_trainer_public_id() {
        pokemon_scoped_lock lock(this);

        return pksav_bigendian16(GEN2_PC_RCAST->ot_id);
    }

    uint16_t pokemon_gen2impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    uint32_t pokemon_gen2impl::get_trainer_id() {
        pokemon_scoped_lock lock(this);

        return uint32_t(pksav_bigendian16(GEN2_PC_RCAST->ot_id));
    }

    void pokemon_gen2impl::set_trainer_public_id(
        uint16_t public_id
    ) {
        pokemon_scoped_lock lock(this);

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen2impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t secret_id)
    ) {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    void pokemon_gen2impl::set_trainer_id(
        uint32_t id
    ) {
        if(id > 65535) {
            throw pkmn::range_error("id", 0, 65535);
        }

        pokemon_scoped_lock lock(this);

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(id));
    }

    std::string pokemon_gen2impl::get_trainer_gender() {
        pokemon_scoped_lock lock(this);

        return (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_OT_GENDER_MASK) ? "Female"
                                                                        : "Male";
    }

    void pokemon_gen2impl::set_trainer_gender(
        const std::string &gender
    ) {
        pokemon_scoped_lock lock(this);

        if(gender == "Male") {
            GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_OT_GENDER_MASK;
        } else if(gender == "Female") {
            GEN2_PC_RCAST->caught_data |= PKSAV_GEN2_OT_GENDER_MASK;
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_gen2impl::get_friendship() {
        pokemon_scoped_lock lock(this);

        return GEN2_PC_RCAST->friendship;
    }

    void pokemon_gen2impl::set_friendship(
        int friendship
    ) {
        if(friendship < 0 or friendship > 255) {
            throw pkmn::range_error("friendship", 0, 255);
        }

        pokemon_scoped_lock lock(this);

        GEN2_PC_RCAST->friendship = uint8_t(friendship);
    }

    std::string pokemon_gen2impl::get_ability() {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    void pokemon_gen2impl::set_ability(
        PKMN_UNUSED(const std::string &ability)
    ) {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    std::string pokemon_gen2impl::get_ball() {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_ball(
        PKMN_UNUSED(const std::string &ball)
    ) {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    int pokemon_gen2impl::get_level_met() {
        pokemon_scoped_lock lock(this);

        return (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_LEVEL_CAUGHT_MASK) >> PKSAV_GEN2_LEVEL_CAUGHT_OFFSET;
    }

    void pokemon_gen2impl::set_level_met(
        int level
    ) {
        if(level < 2 or level > 100) {
            throw pkmn::range_error("Level caught", 2, 100);
        }

        pokemon_scoped_lock lock(this);

        uint16_t caught_data = uint16_t(level);
        caught_data <<= PKSAV_GEN2_LEVEL_CAUGHT_OFFSET;

        GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_LEVEL_CAUGHT_MASK;
        GEN2_PC_RCAST->caught_data |= caught_data;
    }

    std::string pokemon_gen2impl::get_location_met(
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        } else {
            pokemon_scoped_lock lock(this);

            return pkmn::database::location_index_to_name(
                       (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_LOCATION_MASK),
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gen2impl::set_location_met(
        const std::string &location,
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        } else {
            pokemon_scoped_lock lock(this);

            uint16_t location_index = uint16_t(pkmn::database::location_name_to_index(
                                                   location,
                                                   _database_entry.get_game_id()
                                               ));
            GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_LOCATION_MASK;
            GEN2_PC_RCAST->caught_data |= location_index;
        }
    }

    std::string pokemon_gen2impl::get_original_game() {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_original_game(
        PKMN_UNUSED(const std::string &game)
    ) {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    uint32_t pokemon_gen2impl::get_personality() {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    void pokemon_gen2impl::set_personality(
        PKMN_UNUSED(uint32_t personality)
    ) {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    int pokemon_gen2impl::get_experience() {
        pokemon_scoped_lock lock(this);

        uint32_t ret = 0;
        PKSAV_CALL(
            pksav_from_base256(
                GEN2_PC_RCAST->exp,
                3,
                &ret
            );
        )

        return int(ret);
    }

    void pokemon_gen2impl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw pkmn::range_error("experience", 0, max_experience);
        }

        pokemon_scoped_lock lock(this);

        PKSAV_CALL(
            pksav_to_base256(
                experience,
                GEN2_PC_RCAST->exp,
                3
            );
        )

        GEN2_PC_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen2impl::get_level() {
        pokemon_scoped_lock lock(this);

        return int(GEN2_PC_RCAST->level);
    }

    void pokemon_gen2impl::set_level(
        int level
    ) {
        if(level < 2 or level > 100) {
            throw pkmn::range_error("level", 2, 100);
        }

        pokemon_scoped_lock lock(this);

        GEN2_PC_RCAST->level = uint8_t(level);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                GEN2_PC_RCAST->exp,
                3
            );
        )

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen2impl::set_IV(
        const std::string &stat,
        int value
    ) {
        _set_gb_IV(
            stat,
            value,
            &GEN2_PC_RCAST->iv_data
        );

        if(_database_entry.get_species_id() == UNOWN_ID) {
            pokemon_scoped_lock lock(this);
            _set_unown_form_from_IVs();
        }
    }

    void pokemon_gen2impl::set_marking(
        PKMN_UNUSED(const std::string &marking),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Markings", "Generation II");
    }

    void pokemon_gen2impl::set_ribbon(
        PKMN_UNUSED(const std::string &ribbon),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation II");
    }

    void pokemon_gen2impl::set_contest_stat(
        PKMN_UNUSED(const std::string &stat),
        PKMN_UNUSED(int value)
    ) {
        throw pkmn::feature_not_in_game_error("Contests", "Generation II");
    }

    void pokemon_gen2impl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            throw pkmn::range_error("index", 0, 3);
        }

        pokemon_scoped_lock lock(this);

        // This will throw an error if the move is invalid
        _moves[index].move = pkmn::database::move_entry(
                                 move,
                                 get_game()
                             );
        _moves[index].pp = _moves[index].move.get_pp(0);

        GEN2_PC_RCAST->moves[index] = uint8_t(_moves[index].move.get_move_id());
        GEN2_PC_RCAST->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen2impl::_populate_party_data() {
        pksav::gen2_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)
        );

        _update_stat_map();
    }

    void pokemon_gen2impl::set_EV(
        const std::string &stat,
        int value
    ) {
        // Generation II uses Generation I stats for EV's
        if(not pkmn_string_is_gen1_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_EV_in_bounds(value, false)) {
            throw pkmn::range_error(stat, 0, 65535);
        }

        pokemon_scoped_lock lock(this);

        if(stat == "HP") {
            GEN2_PC_RCAST->ev_hp = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Attack") {
            GEN2_PC_RCAST->ev_atk = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Defense") {
            GEN2_PC_RCAST->ev_def = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Speed") {
            GEN2_PC_RCAST->ev_spd = pksav_bigendian16(uint16_t(value));
        } else {
            GEN2_PC_RCAST->ev_spcl = pksav_bigendian16(uint16_t(value));
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_gen2impl::_update_moves(
        int index
    ) {
        if(_moves.size() != 4) {
            _moves.resize(4);
        }
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        GEN2_PC_RCAST->moves[index],
                        _database_entry.get_game_id()
                    ),
                    (GEN2_PC_RCAST->move_pps[index] & PKSAV_GEN2_MOVE_PP_MASK)
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen2impl::_update_held_item() {
        if(int(GEN2_PC_RCAST->held_item) != _held_item.get_item_index()) {
            _held_item = pkmn::database::item_entry(
                             GEN2_PC_RCAST->held_item,
                             _database_entry.get_game_id()
                         );
        }
    }

    void pokemon_gen2impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN2_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN2_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spcl));
    }

    void pokemon_gen2impl::_update_stat_map() {
        _stats["HP"]              = int(pksav_bigendian16(GEN2_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_bigendian16(GEN2_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_bigendian16(GEN2_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_bigendian16(GEN2_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_bigendian16(GEN2_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_bigendian16(GEN2_PARTY_RCAST->spdef));
    }

    void pokemon_gen2impl::_set_unown_form_from_IVs() {
        _database_entry.set_form(
            pkmn::calculations::gen2_unown_form(
                _IVs["Attack"],
                _IVs["Defense"],
                _IVs["Speed"],
                _IVs["Special"]
            )
        );
    }

    void pokemon_gen2impl::_set_unown_IVs_from_form(
        const std::string &form
    ) {
        // Set the maximum possible Special IV for the given form.
        uint16_t num = std::min<uint16_t>(uint16_t((form[0] - 'A') * 10) + 9, 255);

        uint8_t IV_attack = uint8_t(_IVs["Attack"]);
        IV_attack &= ~0x6;
        IV_attack |= ((num & 0xC0) >> 5);

        uint8_t IV_defense = uint8_t(_IVs["Defense"]);
        IV_defense &= ~0x6;
        IV_defense |= ((num & 0x30) >> 3);

        uint8_t IV_speed = uint8_t(_IVs["Speed"]);
        IV_speed &= ~0x6;
        IV_speed |= ((num & 0xC) >> 1);

        uint8_t IV_special = uint8_t(_IVs["Special"]);
        IV_special &= ~0x6;
        IV_special |= ((num & 0x3) << 1);

        _set_gb_IV(
            "Attack",
            IV_attack,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Defense",
            IV_defense,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Speed",
            IV_speed,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Special",
            IV_special,
            &GEN2_PC_RCAST->iv_data
        );
    }
}