/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GC_CAST(ptr)   (static_cast<LibPkmGC::GC::BagData*>(ptr))
#define COLO_CAST(ptr) (static_cast<LibPkmGC::Colosseum::BagData*>(ptr))
#define XD_CAST(ptr)   (static_cast<LibPkmGC::XD::BagData*>(ptr))

#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"

#include "utils/misc.hpp"

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

namespace pkmn {

    item_bag_gcnimpl::item_bag_gcnimpl(
        int game_id,
        const LibPkmGC::GC::BagData* p_libpkmgc_bag
    ): item_bag_impl(game_id)
    {
        const bool is_colosseum = (_game_id == COLOSSEUM_ID);

        if(p_libpkmgc_bag != nullptr)
        {
            _libpkmgc_bag_uptr.reset(p_libpkmgc_bag->clone());
        }
        else
        {
            if(_game_id == COLOSSEUM_ID)
            {
                _libpkmgc_bag_uptr.reset(new LibPkmGC::Colosseum::BagData);
            }
            else
            {
                _libpkmgc_bag_uptr.reset(new LibPkmGC::XD::BagData);
            }
        }

        BOOST_ASSERT(_libpkmgc_bag_uptr.get() != nullptr);

        const int item_pocket_id     = is_colosseum ? 62 : 69;
        const int key_item_pocket_id = is_colosseum ? 63 : 70;
        const int ball_pocket_id     = is_colosseum ? 64 : 71;
        const int tm_pocket_id       = is_colosseum ? 65 : 72;
        const int berry_pocket_id    = is_colosseum ? 66 : 73;
        const int cologne_pocket_id  = is_colosseum ? 67 : 74;

        _item_pockets["Items"] = std::make_shared<item_list_gcnimpl>(
                                     item_pocket_id,
                                     _game_id,
                                     _libpkmgc_bag_uptr->regularItems
                                 );
        _item_pockets["Key Items"] = std::make_shared<item_list_gcnimpl>(
                                         key_item_pocket_id,
                                         _game_id,
                                         _libpkmgc_bag_uptr->keyItems
                                     );
        _item_pockets["Poké Balls"] = std::make_shared<item_list_gcnimpl>(
                                          ball_pocket_id,
                                          _game_id,
                                          _libpkmgc_bag_uptr->pokeballs
                                      );
        _item_pockets["TMs"] = std::make_shared<item_list_gcnimpl>(
                                   tm_pocket_id,
                                   _game_id,
                                   _libpkmgc_bag_uptr->TMs
                               );
        _item_pockets["Berries"] = std::make_shared<item_list_gcnimpl>(
                                       berry_pocket_id,
                                       _game_id,
                                       _libpkmgc_bag_uptr->berries
                                   );
        _item_pockets["Colognes"] = std::make_shared<item_list_gcnimpl>(
                                        cologne_pocket_id,
                                        _game_id,
                                        _libpkmgc_bag_uptr->colognes
                                    );
        if(!is_colosseum)
        {
            static const int BATTLE_CD_POCKET_ID = 91;

            LibPkmGC::XD::BagData* p_xd_bag = dynamic_cast<LibPkmGC::XD::BagData*>(
                                                  _libpkmgc_bag_uptr.get()
                                              );
            BOOST_ASSERT(p_xd_bag != nullptr);

            _item_pockets["Battle CDs"] = std::make_shared<item_list_gcnimpl>(
                                              BATTLE_CD_POCKET_ID,
                                              _game_id,
                                              p_xd_bag->battleCDs
                                          );
        }
    }
}
