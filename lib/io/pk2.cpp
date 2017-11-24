/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pk2.hpp"
#include "../pokemon_gen2impl.hpp"
#include "../database/database_common.hpp"
#include "../database/id_to_index.hpp"

#include <pksav/gen2/pokemon.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    BOOST_STATIC_CONSTEXPR int GOLD = 4;

    bool vector_is_valid_pk2(
        const std::vector<uint8_t>& buffer
    ) {
        // Validate size
        if(buffer.size() != sizeof(pksav_gen2_pc_pokemon_t) and
           buffer.size() != sizeof(pksav_gen2_party_pokemon_t))
        {
            return false;
        }

        const pksav_gen2_pc_pokemon_t* native = reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(buffer.data());

        // Validate species
        try {
            (void)pkmn::database::pokemon_index_to_id(
                      native->species,
                      GOLD
                  );
        } catch(const std::invalid_argument&) {
            return false;
        }

        return true;
    }

    pkmn::pokemon::sptr load_pk2(
        const std::vector<uint8_t>& buffer
    ) {
        if(not vector_is_valid_pk2(buffer)) {
            throw std::runtime_error("Invalid .pk2.");
        }

        return pkmn::make_shared<pokemon_gen2impl>(
                   *reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(buffer.data()),
                   GOLD
               );
    }

    pkmn::pokemon::sptr load_pk2(
        const std::string& filepath
    ) {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument(
                      str(boost::format("The file \"%s\" does not exist.")
                          % filepath.c_str())
                  );
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> buffer(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(buffer.data()), filesize);
        ifile.close();

        return load_pk2(buffer);
    }
}}