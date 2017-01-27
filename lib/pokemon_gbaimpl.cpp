/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_gbaimpl.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <pksav/common/gen3_ribbons.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>

#define GBA_PC_RCAST    reinterpret_cast<pksav_gba_pc_pokemon_t*>(_native_pc)
#define GBA_PARTY_RCAST reinterpret_cast<pksav_gba_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

    pokemon_gbaimpl::pokemon_gbaimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gba_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gba_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gba_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gba_pokemon_party_data_t));
        _our_party_mem = true;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // TODO: Use PKSav PRNG after refactor merged in
        std::srand((unsigned int)std::time(NULL));
        GBA_PC_RCAST->personality = uint32_t(std::rand());
        GBA_PC_RCAST->ot_id.id = pksav_littleendian32(LIBPKMN_OT_ID);

        PKSAV_CALL(
            pksav_text_to_gba(
                boost::algorithm::to_upper_copy(
                    _database_entry.get_name()
                ).c_str(),
                GBA_PC_RCAST->nickname,
                10
            );
        )

        // TODO: language (should be enum in PKSav)

        PKSAV_CALL(
            pksav_text_to_gba(
                LIBPKMN_OT_NAME.c_str(),
                GBA_PC_RCAST->otname,
                7
            );
        )

        _growth->species = pksav_littleendian16(uint16_t(_database_entry.get_pokemon_index()));
        _growth->exp = pksav_littleendian32(uint32_t(
                           _database_entry.get_experience_at_level(level)
                       ));
        _growth->friendship = uint8_t(_database_entry.get_base_friendship());

        // TODO: Use PKSav PRNG after refactor merged in
        _effort->ev_hp    = uint32_t(std::rand());
        _effort->ev_atk   = uint32_t(std::rand());
        _effort->ev_def   = uint32_t(std::rand());
        _effort->ev_spd   = uint32_t(std::rand());
        _effort->ev_spatk = uint32_t(std::rand());
        _effort->ev_spdef = uint32_t(std::rand());

        set_location_met("Fateful encounter", false);
        set_level_met(level);
        set_original_game(get_game());
        set_ball("Premier Ball");

        _misc->iv_egg_ability = uint32_t(std::rand());
        _misc->iv_egg_ability &= ~PKSAV_GBA_EGG_MASK;
        if(GBA_PC_RCAST->personality % 2) {
            _misc->iv_egg_ability |= PKSAV_GBA_ABILITY_MASK;
        } else {
            _misc->iv_egg_ability &= ~PKSAV_GBA_ABILITY_MASK;
        }

        _misc->ribbons_obedience |= PKSAV_GBA_OBEDIENCE_MASK;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_misc->iv_egg_ability);
        _init_contest_stat_map(&_effort->contest_stats);
        _init_markings_map(&GBA_PC_RCAST->markings);
        set_level(level);
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        pksav_gba_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc->blocks.growth.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gba_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_misc->iv_egg_ability);
        _init_contest_stat_map(&_effort->contest_stats);
        _init_markings_map(&GBA_PC_RCAST->markings);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        pksav_gba_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party->pc.blocks.growth.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_misc->iv_egg_ability);
        _init_contest_stat_map(&_effort->contest_stats);
        _init_markings_map(&GBA_PC_RCAST->markings);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        const pksav_gba_pc_pokemon_t &pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc.blocks.growth.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gba_pc_pokemon_t);
        *GBA_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gba_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _init_modern_IV_map(&_misc->iv_egg_ability);
        _init_contest_stat_map(&_effort->contest_stats);
        _init_markings_map(&GBA_PC_RCAST->markings);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    pokemon_gbaimpl::~pokemon_gbaimpl() {
        if(_our_pc_mem) {
            delete GBA_PC_RCAST;
        }
        if(_our_party_mem) {
            delete GBA_PARTY_RCAST;
        }
    }

    void pokemon_gbaimpl::set_form(
        const std::string &form
    ) {
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    std::string pokemon_gbaimpl::get_nickname() {
        pokemon_scoped_lock lock(this);

        char nickname[11] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                GBA_PC_RCAST->nickname,
                nickname,
                10
            );
        )

        return std::string(nickname);
    }

    void pokemon_gbaimpl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        pokemon_scoped_lock lock(this);

        PKSAV_CALL(
            pksav_text_to_gba(
                nickname.c_str(),
                GBA_PC_RCAST->nickname,
                10
            );
        )
    }

    bool pokemon_gbaimpl::is_shiny() {
        pokemon_scoped_lock lock(this);

        return pkmn::calculations::modern_shiny(
                   pksav_littleendian32(GBA_PC_RCAST->personality),
                   pksav_littleendian32(GBA_PC_RCAST->ot_id.id)
               );
    }

    void pokemon_gbaimpl::set_shininess(
        bool value
    ) {
        pokemon_scoped_lock lock(this);

        _set_modern_shininess(
            &GBA_PC_RCAST->personality,
            &GBA_PC_RCAST->ot_id.id,
            value
        );

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_personality();
        }
    }

    void pokemon_gbaimpl::set_held_item(
        const std::string &held_item
    ) {
        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(not item.holdable()) {
            throw std::invalid_argument("This item is not holdable.");
        }

        pokemon_scoped_lock lock(this);

        _growth->held_item = pksav_littleendian16(uint16_t(item.get_item_index()));

        _update_held_item();
    }

    std::string pokemon_gbaimpl::get_trainer_name() {
        pokemon_scoped_lock lock(this);

        char otname[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                GBA_PC_RCAST->otname,
                otname,
                7
            );
        )

        return std::string(otname);
    }

    void pokemon_gbaimpl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name length must be 1-7."
                  );
        }

        pokemon_scoped_lock lock(this);

        PKSAV_CALL(
            pksav_text_to_gba(
                trainer_name.c_str(),
                GBA_PC_RCAST->otname,
                7
            );
        )
    }

    uint16_t pokemon_gbaimpl::get_trainer_public_id() {
        pokemon_scoped_lock lock(this);

        return pksav_littleendian16(GBA_PC_RCAST->ot_id.pid);
    }

    uint16_t pokemon_gbaimpl::get_trainer_secret_id() {
        pokemon_scoped_lock lock(this);

        return pksav_littleendian16(GBA_PC_RCAST->ot_id.sid);
    }

    uint32_t pokemon_gbaimpl::get_trainer_id() {
        pokemon_scoped_lock lock(this);

        return pksav_littleendian32(GBA_PC_RCAST->ot_id.id);
    }

    void pokemon_gbaimpl::set_trainer_public_id(
        uint16_t public_id
    ) {
        pokemon_scoped_lock lock(this);

        GBA_PC_RCAST->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_gbaimpl::set_trainer_secret_id(
        uint16_t secret_id
    ) {
        pokemon_scoped_lock lock(this);

        GBA_PC_RCAST->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_gbaimpl::set_trainer_id(
        uint32_t id
    ) {
        pokemon_scoped_lock lock(this);

        GBA_PC_RCAST->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_gbaimpl::get_trainer_gender() {
        pokemon_scoped_lock lock(this);

        return (_misc->origin_info & PKSAV_GBA_OTGENDER_MASK) ? "Female"
                                                              : "Male";
    }

    void pokemon_gbaimpl::set_trainer_gender(
        const std::string &gender
    ) {
        pokemon_scoped_lock lock(this);

        if(gender == "Male") {
            _misc->origin_info &= ~PKSAV_GBA_OTGENDER_MASK;
        } else if(gender == "Female") {
            _misc->origin_info |= PKSAV_GBA_OTGENDER_MASK;
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_gbaimpl::get_friendship() {
        pokemon_scoped_lock lock(this);

        return _growth->friendship;
    }

    void pokemon_gbaimpl::set_friendship(
        int friendship
    ) {
        if(friendship < 0 or friendship > 255) {
            throw pkmn::range_error("friendship", 0 , 255);
        }

        pokemon_scoped_lock lock(this);

        _growth->friendship = uint8_t(friendship);
    }

    std::string pokemon_gbaimpl::get_ability() {
        pokemon_scoped_lock lock(this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(abilities.second == "None") {
            return abilities.first;
        } else {
            return (_misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK) ? abilities.second
                                                                    : abilities.first;
        }
    }

    void pokemon_gbaimpl::set_ability(
        const std::string &ability
    ) {
        pokemon_scoped_lock lock(this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(ability == "None") {
            throw std::invalid_argument("The ability cannot be set to None.");
        } else if(ability == abilities.first) {
            _misc->iv_egg_ability &= ~PKSAV_GBA_ABILITY_MASK;
        } else if(ability == abilities.second) {
            _misc->iv_egg_ability |= PKSAV_GBA_ABILITY_MASK;
        } else {
            std::string error_message;
            if(abilities.second == "None") {
                error_message = str(boost::format("ability: valid values \"%s\"")
                                    % abilities.first.c_str());
            } else {
                error_message = str(boost::format("ability: valid values \"%s\", \"%s\"")
                                    % abilities.first.c_str()
                                    % abilities.second.c_str());
            }

            throw std::invalid_argument(error_message.c_str());
        }
    }

    std::string pokemon_gbaimpl::get_ball() {
        pokemon_scoped_lock lock(this);

        uint16_t ball = _misc->origin_info & PKSAV_GBA_BALL_MASK;
        ball >>= PKSAV_GBA_BALL_OFFSET;

        return pkmn::database::ball_id_to_name(ball);
    }

    void pokemon_gbaimpl::set_ball(
        const std::string &ball
    ) {
        // Try and instantiate an item_entry to validate the ball.
        (void)pkmn::database::item_entry(ball, get_game());

        pokemon_scoped_lock lock(this);

        _misc->origin_info &= ~PKSAV_GBA_BALL_MASK;
        uint16_t ball_id = uint16_t(pkmn::database::ball_name_to_id(
                                        ball
                                    ));
        _misc->origin_info |= (ball_id << PKSAV_GBA_BALL_OFFSET);
    }


    int pokemon_gbaimpl::get_level_met() {
        pokemon_scoped_lock lock(this);

        return (_misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK);
    }

    void pokemon_gbaimpl::set_level_met(
        int level
    ) {
        if(level < 0 or level > 100) {
            throw pkmn::range_error("Level caught", 0, 100);
        }

        pokemon_scoped_lock lock(this);

        _misc->origin_info &= ~PKSAV_GBA_LEVEL_MET_MASK;
        _misc->origin_info |= uint16_t(level);
    }

    std::string pokemon_gbaimpl::get_location_met(
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        } else {
            pokemon_scoped_lock lock(this);

            return pkmn::database::location_index_to_name(
                       _misc->met_location,
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gbaimpl::set_location_met(
        const std::string &location,
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        } else {
            pokemon_scoped_lock lock(this);

            _misc->met_location = uint8_t(pkmn::database::location_name_to_index(
                                              location,
                                              _database_entry.get_game_id()
                                          ));
        }
    }

    std::string pokemon_gbaimpl::get_original_game() {
        pokemon_scoped_lock lock(this);

        uint16_t original_game = _misc->origin_info & PKSAV_GBA_ORIGIN_GAME_MASK;
        original_game >>= PKSAV_GBA_ORIGIN_GAME_OFFSET;

        return pkmn::database::game_index_to_name(original_game);
    }

    void pokemon_gbaimpl::set_original_game(
        const std::string &game
    ) {
        pokemon_scoped_lock lock(this);

        _misc->origin_info &= ~PKSAV_GBA_ORIGIN_GAME_MASK;
        uint16_t game_index = uint16_t(pkmn::database::game_name_to_index(
                                           game
                                       ));

        _misc->origin_info |= (game_index << PKSAV_GBA_ORIGIN_GAME_OFFSET);
    }

    uint32_t pokemon_gbaimpl::get_personality() {
        pokemon_scoped_lock lock(this);

        return pksav_littleendian32(GBA_PC_RCAST->personality);
    }

    // TODO: automatically update personality-based stuff
    void pokemon_gbaimpl::set_personality(
        uint32_t personality
    ) {
        pokemon_scoped_lock lock(this);

        GBA_PC_RCAST->personality = pksav_littleendian32(personality);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_personality();
        }
    }

    int pokemon_gbaimpl::get_experience() {
        pokemon_scoped_lock lock(this);

        return int(pksav_littleendian32(_growth->exp));
    }

    void pokemon_gbaimpl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw pkmn::range_error("experience", 0, max_experience);
        }

        pokemon_scoped_lock lock(this);

        _growth->exp = pksav_littleendian32(uint32_t(experience));
        GBA_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gbaimpl::get_level() {
        pokemon_scoped_lock lock(this);

        return int(GBA_PARTY_RCAST->level);
    }

    void pokemon_gbaimpl::set_level(
        int level
    ) {
        if(level < 1 or level > 100) {
            throw pkmn::range_error("level", 1, 100);
        }

        pokemon_scoped_lock lock(this);

        GBA_PARTY_RCAST->level = uint8_t(level);
        _growth->exp = pksav_littleendian32(uint32_t(_database_entry.get_experience_at_level(level)));

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gbaimpl::set_IV(
        const std::string &stat,
        int value
    ) {
        _set_modern_IV(
            stat,
            value,
            &_misc->iv_egg_ability
        );
    }

    void pokemon_gbaimpl::set_marking(
        const std::string &marking,
        bool value
    ) {
        _set_marking(
            marking,
            value,
            &GBA_PC_RCAST->markings
        );
    }

    static const std::map<std::string, pksav_gen3_ribbon_mask_t> gba_ribbons = boost::assign::map_list_of
        ("Champion", PKSAV_GEN3_CHAMPION_RIBBON_MASK)
        ("Winning",  PKSAV_GEN3_WINNING_RIBBON_MASK)
        ("Victory",  PKSAV_GEN3_VICTORY_RIBBON_MASK)
        ("Artist",   PKSAV_GEN3_ARTIST_RIBBON_MASK)
        ("Effort",   PKSAV_GEN3_EFFORT_RIBBON_MASK)
        ("Marine",   PKSAV_GEN3_MARINE_RIBBON_MASK)
        ("Land",     PKSAV_GEN3_LAND_RIBBON_MASK)
        ("Sky",      PKSAV_GEN3_SKY_RIBBON_MASK)
        ("Country",  PKSAV_GEN3_COUNTRY_RIBBON_MASK)
        ("National", PKSAV_GEN3_NATIONAL_RIBBON_MASK)
        ("Earth",    PKSAV_GEN3_EARTH_RIBBON_MASK)
        ("World",    PKSAV_GEN3_WORLD_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbon_level_t> gba_contest_ribbon_levels = boost::assign::map_list_of
        ("",       PKSAV_GEN3_CONTEST_RIBBON_NONE)
        ("Normal", PKSAV_GEN3_CONTEST_RIBBON_NORMAL)
        ("Super",  PKSAV_GEN3_CONTEST_RIBBON_SUPER)
        ("Hyper",  PKSAV_GEN3_CONTEST_RIBBON_HYPER)
        ("Master", PKSAV_GEN3_CONTEST_RIBBON_MASTER)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_mask_t> gba_contest_ribbon_masks = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_MASK)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_MASK)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_MASK)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_MASK)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_offset_t> gba_contest_ribbon_offsets = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_OFFSET)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_OFFSET)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_OFFSET)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_OFFSET)
    ;

    void pokemon_gbaimpl::set_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        if(_ribbons.find(ribbon) == _ribbons.end()) {
            throw std::invalid_argument("Invalid ribbon.");
        }

        pokemon_scoped_lock lock(this);

        if(gba_ribbons.find(ribbon) != gba_ribbons.end()) {
            _set_ribbon<uint32_t, pksav_gen3_ribbon_mask_t>(
                ribbon,
                value,
                &_misc->ribbons_obedience,
                gba_ribbons
            );
        } else {
            _set_contest_ribbon(
                ribbon,
                value
            );
        }
    }

    void pokemon_gbaimpl::set_contest_stat(
        const std::string &stat,
        int value
    ) {
        _set_contest_stat(
            stat,
            value,
            &_effort->contest_stats
        );
    }

    void pokemon_gbaimpl::set_move(
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

        _attacks->moves[index] = pksav_littleendian16(uint16_t(_moves[index].move.get_move_id()));
        _attacks->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gbaimpl::set_EV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_modern_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_EV_in_bounds(value, true)) {
            throw pkmn::range_error(stat, 0, 255);
        }

        pokemon_scoped_lock lock(this);

        if(stat == "HP") {
            _effort->ev_hp = uint8_t(value);
        } else if(stat == "Attack") {
            _effort->ev_atk = uint8_t(value);
        } else if(stat == "Defense") {
            _effort->ev_def = uint8_t(value);
        } else if(stat == "Speed") {
            _effort->ev_spd = uint8_t(value);
        } else if(stat == "Special Attack") {
            _effort->ev_spatk = uint8_t(value);
        } else {
            _effort->ev_spdef = uint8_t(value);
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_gbaimpl::_set_contest_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        std::vector<std::string> ribbon_parts;
        boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));

        // Validate input (which should already have been validated)
        if((ribbon_parts.size() == 0 or ribbon_parts.size() > 2) or
           (gba_contest_ribbon_masks.count(ribbon_parts.at(0)) == 0) or
           (ribbon_parts.size() == 2 and
            gba_contest_ribbon_levels.count(ribbon_parts.at(1)) == 0)
          )
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        uint16_t mask = uint16_t(gba_contest_ribbon_masks.at(ribbon_parts.at(0)));
        uint16_t offset = uint16_t(gba_contest_ribbon_offsets.at(ribbon_parts.at(0)));
        uint16_t level = 0;
        if(ribbon_parts.size() == 2) {
            level = uint16_t(gba_contest_ribbon_levels.at(ribbon_parts.at(1)));
        }

        uint16_t current_level = (_misc->ribbons_obedience & mask) >> offset;
        if(value) {
            if(level == 0) {
                ++level;
            }
            level = std::max<uint16_t>(level, current_level);
        } else {
            if(level > 0) {
                --level;
            }
            level = std::min<uint16_t>(level, current_level);
        }

        _misc->ribbons_obedience &= ~mask;
        _misc->ribbons_obedience |= (level << offset);

        _update_ribbons_map();
    }

    void pokemon_gbaimpl::_populate_party_data() {
        pksav::gba_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gba_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gba_pokemon_party_data_t*>(_native_party)
        );

        _update_stat_map();
    }

    void pokemon_gbaimpl::_update_moves(
        int index
    ) {
        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_attacks->moves[index]),
                        _database_entry.get_game_id()
                    ),
                    _attacks->move_pps[index]
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gbaimpl::_update_held_item() {
        if(int(pksav_littleendian16(_growth->held_item)) != _held_item.get_item_index()) {
            _held_item = pkmn::database::item_entry(
                             pksav_littleendian16(_growth->held_item),
                             _database_entry.get_game_id()
                         );
        }
    }

    void pokemon_gbaimpl::_update_markings_map() {
        _markings["Circle"]   = bool(GBA_PC_RCAST->markings & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool(GBA_PC_RCAST->markings & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool(GBA_PC_RCAST->markings & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool(GBA_PC_RCAST->markings & PKSAV_MARKING_HEART);
    }

    void pokemon_gbaimpl::_update_ribbons_map() {
        uint32_t cool_ribbons = _misc->ribbons_obedience & PKSAV_GEN3_COOL_RIBBONS_MASK;
        _ribbons["Cool"]        = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Cool Super"]  = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Cool Hyper"]  = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Cool Master"] = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t beauty_ribbons = _misc->ribbons_obedience & PKSAV_GEN3_BEAUTY_RIBBONS_MASK;
        beauty_ribbons >>= PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET;
        _ribbons["Beauty"]        = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Beauty Super"]  = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Beauty Hyper"]  = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Beauty Master"] = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t cute_ribbons = _misc->ribbons_obedience & PKSAV_GEN3_CUTE_RIBBONS_MASK;
        cute_ribbons >>= PKSAV_GEN3_CUTE_RIBBONS_OFFSET;
        _ribbons["Cute"]        = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Cute Super"]  = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Cute Hyper"]  = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Cute Master"] = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t smart_ribbons = _misc->ribbons_obedience & PKSAV_GEN3_SMART_RIBBONS_MASK;
        smart_ribbons >>= PKSAV_GEN3_SMART_RIBBONS_OFFSET;
        _ribbons["Smart"]        = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Smart Super"]  = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Smart Hyper"]  = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Smart Master"] = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t tough_ribbons = _misc->ribbons_obedience & PKSAV_GEN3_TOUGH_RIBBONS_MASK;
        tough_ribbons >>= PKSAV_GEN3_TOUGH_RIBBONS_OFFSET;
        _ribbons["Tough"]        = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Tough Super"]  = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Tough Hyper"]  = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Tough Master"] = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        _ribbons["Champion"] = bool(_misc->ribbons_obedience & PKSAV_GEN3_CHAMPION_RIBBON_MASK);
        _ribbons["Winning"]  = bool(_misc->ribbons_obedience & PKSAV_GEN3_WINNING_RIBBON_MASK);
        _ribbons["Victory"]  = bool(_misc->ribbons_obedience & PKSAV_GEN3_VICTORY_RIBBON_MASK);
        _ribbons["Artist"]   = bool(_misc->ribbons_obedience & PKSAV_GEN3_ARTIST_RIBBON_MASK);
        _ribbons["Effort"]   = bool(_misc->ribbons_obedience & PKSAV_GEN3_EFFORT_RIBBON_MASK);
        _ribbons["Marine"]   = bool(_misc->ribbons_obedience & PKSAV_GEN3_MARINE_RIBBON_MASK);
        _ribbons["Land"]     = bool(_misc->ribbons_obedience & PKSAV_GEN3_LAND_RIBBON_MASK);
        _ribbons["Sky"]      = bool(_misc->ribbons_obedience & PKSAV_GEN3_SKY_RIBBON_MASK);
        _ribbons["Country"]  = bool(_misc->ribbons_obedience & PKSAV_GEN3_COUNTRY_RIBBON_MASK);
        _ribbons["National"] = bool(_misc->ribbons_obedience & PKSAV_GEN3_NATIONAL_RIBBON_MASK);
        _ribbons["Earth"]    = bool(_misc->ribbons_obedience & PKSAV_GEN3_EARTH_RIBBON_MASK);
        _ribbons["World"]    = bool(_misc->ribbons_obedience & PKSAV_GEN3_WORLD_RIBBON_MASK);
    }

    void pokemon_gbaimpl::_update_EV_map() {
        _EVs["HP"]              = int(_effort->ev_hp);
        _EVs["Attack"]          = int(_effort->ev_atk);
        _EVs["Defense"]         = int(_effort->ev_def);
        _EVs["Speed"]           = int(_effort->ev_spd);
        _EVs["Special Attack"]  = int(_effort->ev_spatk);
        _EVs["Special Defense"] = int(_effort->ev_spdef);
    }

    void pokemon_gbaimpl::_update_stat_map() {
        _stats["HP"]              = int(pksav_littleendian16(GBA_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(GBA_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(GBA_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(GBA_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(GBA_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(GBA_PARTY_RCAST->spdef));
    }

    void pokemon_gbaimpl::_set_unown_form_from_personality() {
        _database_entry.set_form(
            pkmn::calculations::gen3_unown_form(
                pksav_littleendian32(
                    GBA_PC_RCAST->personality
                )
            )
        );
    }

    void pokemon_gbaimpl::_set_unown_personality_from_form() {
        char as_char = _database_entry.get_form()[0];
        uint8_t num = 0;

        switch(as_char) {
            case '?':
                num = 26;
                break;

            case '!':
                num = 27;
                break;

            // We can assume the form is valid at this point.
            default:
                num = uint8_t(as_char - 'A');
                break;
        }

        uint8_t* pid_as_bytes = reinterpret_cast<uint8_t*>(&GBA_PC_RCAST->personality);
        for(size_t i = 0; i < 4; ++i) {
            pid_as_bytes[i] &= ~0x3;
            pid_as_bytes[i] |= ((num & (0x3 << (2*i))) >> (2*i));
        }
    }
}
