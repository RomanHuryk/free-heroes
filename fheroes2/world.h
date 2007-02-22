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
#ifndef H2MAPSDATA_H
#define H2MAPSDATA_H

#include <vector>
#include "gamedefs.h"
#include "surface.h"
#include "kingdom.h"
#include "rect.h"
#include "maps.h"
#include "mp2.h"

#define DAYOFWEEK       7
#define WEEKOFMONTH     4

class World
{
public:
    static World & GetWorld(void){ return world; };
    void LoadMaps(const std::string &filename);

    u16 w(void) const{ return width; };
    u16 h(void) const{ return height; };

    const Surface & GetSpriteMaps(void){ return *sprite_maps; };
    u16 GetGround(u16 index) const{ return index < vec_tiles.size() ? vec_tiles[index].ground : 0; };
    u8  GetObject(u16 index) const{ return index < vec_tiles.size() ? vec_tiles[index].object : 0; };

    bool Movement(u16 ax, u16 ay) const{ return Movement(ax * width + ay); };
    bool Movement(u16 index) const;

    void Redraw(const Rect &rt, const Point &pt = Point(0, 0)) const;

    u8 GetDay(void){ return (day % DAYOFWEEK) + 1; };
    u8 GetWeek(void){ return (week % WEEKOFMONTH) + 1; };
    u8 GetMonth(void){ return month; };
    u16 CountDay(void){ return day; };
    u16 CountWeek(void){ return week; };
    bool BeginWeek(void){ return begin_week; };
    bool BeginMonth(void){ return begin_month; };
    void ResetDate(void);
    void NextDay(void);

    const Kingdom & GetKingdom(Color::color_t color);

private:
    World() : sprite_maps(NULL), kingdom(), width(0), height(0) {};
    ~World();

    typedef struct { Point coord; Surface *sprite; } levelsprite_t;

    Surface *sprite_maps;
    std::vector<Maps::tiles_t> vec_tiles;

    Kingdom kingdom[KINGDOMMAX + 1];

    u16 width;
    u16 height;

    u16 day;
    u16 week;
    u8 month;
    bool begin_week;
    bool begin_month;
    
    static World world;
};

#endif
