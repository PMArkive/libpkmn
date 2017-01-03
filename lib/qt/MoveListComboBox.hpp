/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_MOVELISTCOMBOBOX_HPP
#define PKMN_QT_MOVELISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtGui/QComboBox>
#include <QtCore/QString>
#else
#include <QComboBox>
#include <QString>
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with a list of moves (sorted by in-game index) of
     *        moves available in the given game.
     */
    class PKMN_API MoveListComboBox: public QComboBox {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * \param game which game
             * \param parent parent widget
             * \throws std::invalid_argument if the given game is invalid
             */
            MoveListComboBox(
                const QString &game,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_MOVELISTCOMBOBOX_HPP */
