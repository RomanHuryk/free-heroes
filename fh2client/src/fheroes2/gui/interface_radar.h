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

#ifndef H2INTERFACE_RADAR_H
#define H2INTERFACE_RADAR_H

#include "interface_border.h"

namespace Interface
{
    class Basic;

    class Radar : public BorderWindow
    {
    public:
	Radar(Basic &);

	void SetPos(s16, s16);
	void SavePosition(void);
	void SetRedraw(void) const;

	void Build(void);
	void Generate(void);
	void Redraw(void);
	void RedrawArea(const u8 color = 0xFF);
	void SetHide(bool);
	void RedrawCursor(void);

	void QueueEventProcessing(void);

    private:
	Surface* GetSurfaceFromColor(u8);

	Basic & interface;

        Surface spriteArea;
	SpriteMove cursorArea;

        Surface sf_blue;
	Surface sf_green;
        Surface sf_red;
	Surface sf_yellow;
        Surface sf_orange;
	Surface sf_purple;
        Surface sf_gray;
	Surface sf_black;

	bool hide;
    };
}

#endif
