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
#ifndef H2KINGDOM_H
#define H2KINGDOM_H

#include <vector>
#include <map>
#include "race.h"
#include "color.h"
#include "resource.h"
#include "gamedefs.h"
#include "game.h"
#include "mp2.h"
#include "pairs.h"
#include "game_io.h"
#include "heroes_recruits.h"

class Castle;
class Heroes;

class Kingdom
{
public:
    enum flags_t
    {
	PLAY	= 0x0001,
    };

    Kingdom(const Color::color_t cl, const Game::control_t con = Game::AI);

    void SetModes(flags_t);
    void ResetModes(flags_t);
    bool Modes(flags_t) const;

    Game::control_t Control() const{ return control; };
    void SetControl(const Game::control_t con) { control = con; };
    void UpdateStartingResource(void);
    bool isPlay(void) const{ return Modes(PLAY); };
    bool isLoss(void) const;
    bool AllowPayment(const Resource::funds_t & funds) const;

    Color::color_t GetColor(void) const{ return color; };
    Race::race_t GetRace(void) const;

    const Resource::funds_t & GetFundsResource(void) const{ return resource; };
    u16 GetFundsWood(void) const{ return resource.wood; };
    u16 GetFundsMercury(void) const{ return resource.mercury; };
    u16 GetFundsOre(void) const{ return resource.ore; };
    u16 GetFundsSulfur(void) const{ return resource.sulfur; };
    u16 GetFundsCrystal(void) const{ return resource.crystal; };
    u16 GetFundsGems(void) const{ return resource.gems; };
    u32 GetFundsGold(void) const{ return resource.gold; };

    void AddFundsResource(const Resource::funds_t & funds);
    void OddFundsResource(const Resource::funds_t & funds);

    u8 GetCountCastle(void) const;
    u8 GetCountTown(void) const;
    u8 GetCountMarketplace(void) const;
    u8 GetCountCapital(void) const;
    u8 GetLostTownDays(void) const;

    Recruits & GetRecruits(void);

    const std::vector<Heroes *> & GetHeroes(void) const{ return heroes; };
    const std::vector<Castle *> & GetCastles(void) const{ return castles; };

    std::vector<Heroes *> & GetHeroes(void) { return heroes; };
    std::vector<Castle *> & GetCastles(void) { return castles; };

    void AddHeroes(const Heroes *hero);
    void RemoveHeroes(const Heroes *hero);
    void FreeAllHeroes(void);

    void AddCastle(const Castle *castle);
    void RemoveCastle(const Castle *castle);
    
    void AITurns(void);
    void ActionNewDay(void);
    void ActionNewWeek(void);
    void ActionNewMonth(void);

    void SetVisited(const u16 index, const MP2::object_t object = MP2::OBJ_ZERO);
    u16  CountVisitedObjects(const MP2::object_t) const;
    bool isVisited(const u8 object) const;
    bool isVisited(const Maps::Tiles & tile) const;

    bool HeroesMayStillMove(void) const;

    void Dump(void) const;

private:
    friend void Game::SaveXML(const std::string &);
    friend void Game::LoadXML(const std::string &);

    void UpdateRecruits(void);

    void AICastlesTurns(void);
    void AIHeroesTurns(Heroes &);
    void AIHeroesTask(Heroes &);
    void AIDumpCacheObjects(const IndexDistance &) const;

    const Color::color_t color;
    Game::control_t control;
    Resource::funds_t resource;

    u16 flags;
    u8  lost_town_days;

    std::vector<Castle *> castles;
    std::vector<Heroes *> heroes;

    Recruits recruits;

    Castle *ai_capital;
    std::map<u16, MP2::object_t> ai_objects;

    std::list<IndexObject> visit_object;
};

#endif
