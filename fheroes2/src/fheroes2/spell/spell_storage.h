/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef H2SPELLSTORAGE_H
#define H2SPELLSTORAGE_H

#include <vector>
#include "spell.h"
#include "game_io.h"
#include "gamedefs.h"

class SpellStorage
{
public:
	SpellStorage();

	u8 Size(u8 lvl = 0) const;

	const std::vector<Spell::spell_t> & Spells(void) const;
	void GetSpells(std::vector<Spell::spell_t> &, u8) const;

	void Appends(const SpellStorage & st, const u8 wisdom);
	void Append(const Spell::spell_t sp, const u8 wisdom);
	void Append(const Spell::spell_t);

	bool isPresentSpell(Spell::spell_t) const;

protected:
	friend class Game::IO;

	std::vector<Spell::spell_t> spells;
};

#endif
