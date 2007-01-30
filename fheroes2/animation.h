/***************************************************************************
 *   Copyright (C) 2006 by Andrey Afletdinov                               *
 *   afletdinov@mail.dc.baikal.ru                                          *
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
#ifndef H2ANIMATION_H
#define H2ANIMATION_H

#include <vector>
#include "agg.h"
#include "cursor.h"
#include "rect.h"
#include "sprite.h"
#include "gamedefs.h"

class Animation
{
public:
    typedef enum { INFINITY = 0x01, RING = 0x02, LOW = 0x04, MEDIUM = 0x08, HIGH = 0x10 } animatoin_t;

    Animation(const std::string &icn, u16 index, u8 count, u8 amode = INFINITY | RING | MEDIUM);

    void DrawSprite(void);
    void Reset(void);

private:
    Rect area;
    bool disable;
    bool reset;
    u32 frame;
    u32 ticket;
    const u8  mode;
    std::vector<const Sprite *> sprites;
};

#endif
