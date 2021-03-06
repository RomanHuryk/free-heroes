/***************************************************************************
 *   Copyright (C) 2010 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#ifndef H2BATTLE_CELL_H
#define H2BATTLE_CELL_H

#include "gamedefs.h"
#include "battle.h"

#define CELLW 45
#define CELLH 52
#define CELLW2 23
#define CELLH2 26

namespace Battle
{
    class Board;
    class Unit;

    enum direction_t
    {
        UNKNOWN         = 0x00,
        TOP_LEFT        = 0x01,
        TOP_RIGHT       = 0x02,
        RIGHT           = 0x04,
        BOTTOM_RIGHT    = 0x08,
        BOTTOM_LEFT     = 0x10,
        LEFT            = 0x20,
        CENTER          = 0x40,
	RIGHT_SIDE	= TOP_RIGHT | RIGHT | BOTTOM_RIGHT,
	LEFT_SIDE	= TOP_LEFT | LEFT | BOTTOM_LEFT,
	AROUND		= RIGHT_SIDE | LEFT_SIDE
    };

    class Cell
    {
    public:
	Cell();
	Cell(const s16 &);

	void		ResetQuality(void);
	void		ResetDirection(void);

	void		SetObject(u8);
	void		SetDirection(u8);
	void		SetQuality(u32);

	void		SetArea(const Rect &);

	bool		isPassable4(const Unit &, const Cell &) const;
	bool		isPassable3(const Unit &, bool check_reflect) const;
	bool		isPassable1(bool check_troop) const;
	bool		isPositionIncludePoint(const Point &) const;

	const s16 &	GetIndex(void) const;
	const Rect &	GetPos(void) const;
	const u8 &	GetObject(void) const;
	const u8 &	GetDirection(void) const;
	const s32 &	GetQuality(void) const;
	direction_t	GetTriangleDirection(const Point &) const;

	const Unit*	GetUnit(void) const;
	Unit*		GetUnit(void);
	void		SetUnit(Unit*);

    private:
	friend StreamBase & operator<< (StreamBase &, const Cell &);
	friend StreamBase & operator>> (StreamBase &, Cell &);

	s16		index;
	Rect		pos;
	u8		object;
	u8		direction;
	s32		quality;
	Unit*		troop;
	Point		coord[7];
    };

    StreamBase & operator<< (StreamBase &, const Cell &);
    StreamBase & operator>> (StreamBase &, Cell &);

    class Position : protected std::pair<Cell*, Cell*>
    {
    public:
	Position() : std::pair<Cell*, Cell*>(NULL, NULL) {}

	void		Set(s16 head, bool wide, bool reflect);
	void		Swap(void);
	bool		isReflect(void) const;
	bool		isValid(void) const;

	static Position	GetCorrect(const Unit &, s16);

	Rect		GetRect(void) const;
	Cell*		GetHead(void);
	const Cell*	GetHead(void) const;
	Cell*		GetTail(void);
	const Cell*	GetTail(void) const;
    };
}

#endif
