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

#include "ai/ai.h"
 
#include <algorithm>
#include <functional>

#include "army/army.h"
#include "battle/battle.h"
#include "castle/castle.h"
#include "dialog/dialog.h"
#include "game/difficulty.h"
#include "game/game.h"
#include "game/game_interface.h"
#include "gui/cursor.h"
#include "gui/interface_gamearea.h"
#include "heroes/heroes.h"
#include "kingdom/kingdom.h"
#include "kingdom/luck.h"
#include "kingdom/morale.h"
#include "kingdom/payment.h"
#include "kingdom/race.h"
#include "kingdom/world.h"
#include "maps/maps_tiles.h"
#include "system/settings.h"

void AIToMonster(Heroes & hero, u32 obj, s32 dst_index);
void AIToPickupResource(Heroes & hero, u32 obj, s32 dst_index);
void AIToTreasureChest(Heroes & hero, u32 obj, s32 dst_index);
void AIToArtifact(Heroes & hero, u32 obj, s32 dst_index);
void AIToResource(Heroes & hero, u32 obj, s32 dst_index);
void AIToWagon(Heroes & hero, u32 obj, s32 dst_index);
void AIToSkeleton(Heroes & hero, u32 obj, s32 dst_index);
void AIToCaptureObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToFlotSam(Heroes & hero, u32 obj, s32 dst_index);
void AIToObservationTower(Heroes & hero, u32 obj, s32 dst_index);
void AIToMagellanMaps(Heroes & hero, u32 obj, s32 dst_index);
void AIToTeleports(Heroes & hero, s32 dst_index);
void AIToWhirlpools(Heroes & hero, s32 dst_index);
void AIToPrimarySkillObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToExperienceObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToWitchsHut(Heroes & hero, u32 obj, s32 dst_index);
void AIToShrine(Heroes & hero, u32 obj, s32 dst_index);
void AIToGoodMoraleObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToMagicWell(Heroes & hero, u32 obj, s32 dst_index);
void AIToArtesianSpring(Heroes & hero, u32 obj, s32 dst_index);
void AIToXanadu(Heroes & hero, u32 obj, s32 dst_index);
void AIToEvent(Heroes & hero, u32 obj, s32 dst_index);
void AIToUpgradeArmyObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToPoorMoraleObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToPoorLuckObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToGoodLuckObject(Heroes & hero, u32 obj, s32 dst_index);
void AIToObelisk(Heroes & hero, u32 obj, s32 dst_index);
void AIToTreeKnowledge(Heroes & hero, u32 obj, s32 dst_index);
void AIToDaemonCave(Heroes & hero, u32 obj, s32 dst_index);
void AIToCastle(Heroes & hero, u32 obj, s32 dst_index);
void AIToSign(Heroes & hero, u32 obj, s32 dst_index);
void AIToDwellingJoinMonster(Heroes & hero, u32 obj, s32 dst_index);
void AIToHeroes(Heroes & hero, u32 obj, s32 dst_index);
void AIToDwellingRecruitMonster(Heroes & hero, u32 obj, s32 dst_index);
void AIToStables(Heroes & hero, u32 obj, s32 dst_index);
void AIToAbandoneMine(Heroes & hero, u32 obj, s32 dst_index);
void AIToBarrier(Heroes & hero, u32 obj, s32 dst_index);
void AIToTravellersTent(Heroes & hero, u32 obj, s32 dst_index);
void AIToShipwreckSurvivor(Heroes & hero, u32 obj, s32 dst_index);
void AIToBoat(Heroes & hero, u32 obj, s32 dst_index);
void AIToCoast(Heroes & hero, u32 obj, s32 dst_index);
void AIMeeting(Heroes & hero1, Heroes & hero2);

int AISelectPrimarySkill(Heroes &hero)
{
    switch(hero.GetRace())
    {
	case Race::KNGT:
	{
	    if(5 > hero.GetDefense())	return Skill::Primary::DEFENSE;
	    if(5 > hero.GetAttack())	return Skill::Primary::ATTACK;
	    if(3 > hero.GetKnowledge())	return Skill::Primary::KNOWLEDGE;
	    if(3 > hero.GetPower())	return Skill::Primary::POWER;
    	    break;
	}

	case Race::BARB:
	{
	    if(5 > hero.GetAttack())	return Skill::Primary::ATTACK;
	    if(5 > hero.GetDefense())	return Skill::Primary::DEFENSE;
	    if(3 > hero.GetPower())	return Skill::Primary::POWER;
	    if(3 > hero.GetKnowledge())	return Skill::Primary::KNOWLEDGE;
	    break;
	}

	case Race::SORC:
	case Race::WZRD:
	{
	    if(5 > hero.GetKnowledge())	return Skill::Primary::KNOWLEDGE;
	    if(5 > hero.GetPower())	return Skill::Primary::POWER;
	    if(3 > hero.GetDefense())	return Skill::Primary::DEFENSE;
	    if(3 > hero.GetAttack())	return Skill::Primary::ATTACK;
	    break;
	}

	case Race::WRLK:
	case Race::NECR:
	{
	    if(5 > hero.GetPower())	return Skill::Primary::POWER;
	    if(5 > hero.GetKnowledge())	return Skill::Primary::KNOWLEDGE;
	    if(3 > hero.GetAttack())	return Skill::Primary::ATTACK;
	    if(3 > hero.GetDefense())	return Skill::Primary::DEFENSE;
	    break;
	}

	default: break;
    }

    switch(Rand::Get(1,4))
    {
	case 1:	return Skill::Primary::ATTACK;
	case 2:	return Skill::Primary::DEFENSE;
	case 3:	return Skill::Primary::POWER;
	case 4:	return Skill::Primary::KNOWLEDGE;
	default: break;
    }

    return Skill::Primary::UNKNOWN;
}

void AIBattleLose(Heroes &hero, const Battle::Result & res, bool attacker, int color = Color::NONE)
{
    u32 reason = attacker ? res.AttackerResult() : res.DefenderResult();

    if(Settings::Get().ExtHeroSurrenderingGiveExp() &&
	Battle::RESULT_SURRENDER == reason)
    {
        const u32 & exp = attacker ? res.GetExperienceAttacker() : res.GetExperienceDefender();

        if(hero.isControlHuman())
        {
            std::string msg = _("Hero %{name} also got a %{count} experience.");
            StringReplace(msg, "%{name}", hero.GetName());
            StringReplace(msg, "%{count}", exp);
            Dialog::Message("", msg, Font::BIG, Dialog::OK);
        }
        hero.IncreaseExperience(exp);
    }

    hero.SetKillerColor(color);
    hero.SetFreeman(reason);
}

void AI::HeroesAction(Heroes & hero, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);
    const int object = (dst_index == hero.GetIndex() ?
	tile.GetObject(false) : tile.GetObject());

    if(MP2::isActionObject(object, hero.isShipMaster())) hero.SetModes(Heroes::ACTION);

    switch(object)
    {
        case MP2::OBJ_BOAT:		AIToBoat(hero, object, dst_index); break;
        case MP2::OBJ_COAST:		AIToCoast(hero, object, dst_index); break;

    	case MP2::OBJ_MONSTER:		AIToMonster(hero, object, dst_index); break;
	case MP2::OBJ_HEROES:		AIToHeroes(hero, object, dst_index); break;
	case MP2::OBJ_CASTLE:		AIToCastle(hero, object, dst_index); break;

        // pickup object
        case MP2::OBJ_RESOURCE:
        case MP2::OBJ_BOTTLE:
        case MP2::OBJ_CAMPFIRE:         AIToPickupResource(hero, object, dst_index); break;

        case MP2::OBJ_WATERCHEST:
        case MP2::OBJ_TREASURECHEST:	AIToTreasureChest(hero, object, dst_index); break;
	case MP2::OBJ_ARTIFACT:		AIToArtifact(hero, object, dst_index); break;

        case MP2::OBJ_MAGICGARDEN:
        case MP2::OBJ_LEANTO:
        case MP2::OBJ_WINDMILL:
        case MP2::OBJ_WATERWHEEL:	AIToResource(hero, object, dst_index); break;

        case MP2::OBJ_WAGON:		AIToWagon(hero, object, dst_index); break;
        case MP2::OBJ_SKELETON:		AIToSkeleton(hero, object, dst_index); break;
        case MP2::OBJ_FLOTSAM:		AIToFlotSam(hero, object, dst_index); break;

	case MP2::OBJ_ALCHEMYLAB:
        case MP2::OBJ_MINES:
	case MP2::OBJ_SAWMILL:
        case MP2::OBJ_LIGHTHOUSE:	AIToCaptureObject(hero, object, dst_index); break;
        case MP2::OBJ_ABANDONEDMINE:    AIToAbandoneMine(hero, object, dst_index); break;

	case MP2::OBJ_SHIPWRECKSURVIROR:AIToShipwreckSurvivor(hero, object, dst_index); break;

	// event
	case MP2::OBJ_EVENT:		AIToEvent(hero, object, dst_index); break;

	case MP2::OBJ_SIGN:		AIToSign(hero, object, dst_index); break;

	// increase view
	case MP2::OBJ_OBSERVATIONTOWER:	AIToObservationTower(hero, object, dst_index); break;
        case MP2::OBJ_MAGELLANMAPS:	AIToMagellanMaps(hero, object, dst_index); break;

        // teleports
	case MP2::OBJ_STONELIGHTS:	AIToTeleports(hero, dst_index); break;
	case MP2::OBJ_WHIRLPOOL:	AIToWhirlpools(hero, dst_index); break;

        // primary skill modification
        case MP2::OBJ_FORT:
        case MP2::OBJ_MERCENARYCAMP:
        case MP2::OBJ_DOCTORHUT:
        case MP2::OBJ_STANDINGSTONES:	AIToPrimarySkillObject(hero, object, dst_index); break;

	// experience modification
        case MP2::OBJ_GAZEBO:           AIToExperienceObject(hero, object, dst_index); break;

        // witchs hut
        case MP2::OBJ_WITCHSHUT: 	AIToWitchsHut(hero, object, dst_index); break;

        // shrine circle
	case MP2::OBJ_SHRINE1:
	case MP2::OBJ_SHRINE2:
        case MP2::OBJ_SHRINE3:		AIToShrine(hero, object, dst_index); break;

        // luck modification
        case MP2::OBJ_FOUNTAIN:
        case MP2::OBJ_FAERIERING:
        case MP2::OBJ_IDOL:
        case MP2::OBJ_MERMAID:
					AIToGoodLuckObject(hero, object, dst_index); break;

        // morale modification
        case MP2::OBJ_OASIS:
        case MP2::OBJ_TEMPLE:
        case MP2::OBJ_WATERINGHOLE:
        case MP2::OBJ_BUOY:		AIToGoodMoraleObject(hero, object, dst_index); break;

	case MP2::OBJ_OBELISK:		AIToObelisk(hero, object, dst_index); break;

        // magic point
	case MP2::OBJ_ARTESIANSPRING:	AIToArtesianSpring(hero, object, dst_index); break;
        case MP2::OBJ_MAGICWELL: 	AIToMagicWell(hero, object, dst_index); break;

        // increase skill
	case MP2::OBJ_XANADU:		AIToXanadu(hero, object, dst_index); break;

        case MP2::OBJ_HILLFORT:
        case MP2::OBJ_FREEMANFOUNDRY:	AIToUpgradeArmyObject(hero, object, dst_index); break;

	case MP2::OBJ_SHIPWRECK:
        case MP2::OBJ_GRAVEYARD:
	case MP2::OBJ_DERELICTSHIP:	AIToPoorMoraleObject(hero, object, dst_index); break;

	case MP2::OBJ_PYRAMID:		AIToPoorLuckObject(hero, object, dst_index); break;
	case MP2::OBJ_DAEMONCAVE:	AIToDaemonCave(hero, object, dst_index); break;

        case MP2::OBJ_TREEKNOWLEDGE:	AIToTreeKnowledge(hero, object, dst_index); break;

        // accept army
        case MP2::OBJ_WATCHTOWER:
        case MP2::OBJ_EXCAVATION:
        case MP2::OBJ_CAVE:
        case MP2::OBJ_TREEHOUSE:
        case MP2::OBJ_ARCHERHOUSE:
        case MP2::OBJ_GOBLINHUT:
        case MP2::OBJ_DWARFCOTT:
	case MP2::OBJ_HALFLINGHOLE:
        case MP2::OBJ_PEASANTHUT:
        case MP2::OBJ_THATCHEDHUT:      AIToDwellingJoinMonster(hero, object, dst_index); break;

        // recruit army
        case MP2::OBJ_RUINS:
        case MP2::OBJ_TREECITY:
        case MP2::OBJ_WAGONCAMP:
	case MP2::OBJ_DESERTTENT:
        // loyalty ver
        case MP2::OBJ_WATERALTAR:
        case MP2::OBJ_AIRALTAR:
        case MP2::OBJ_FIREALTAR:
        case MP2::OBJ_EARTHALTAR:
        case MP2::OBJ_BARROWMOUNDS:     AIToDwellingRecruitMonster(hero, object, dst_index); break;

        // recruit army (battle)
        case MP2::OBJ_DRAGONCITY:
        case MP2::OBJ_CITYDEAD:
        case MP2::OBJ_TROLLBRIDGE:	AIToDwellingRecruitMonster(hero, object, dst_index); break;

	// recruit genie
	case MP2::OBJ_ANCIENTLAMP:	AIToDwellingRecruitMonster(hero, object, dst_index); break;

	case MP2::OBJ_STABLES:		AIToStables(hero, object, dst_index); break;
	case MP2::OBJ_ARENA:		AIToPrimarySkillObject(hero, object, dst_index); break;

        case MP2::OBJ_BARRIER:          AIToBarrier(hero, object, dst_index); break;
        case MP2::OBJ_TRAVELLERTENT:    AIToTravellersTent(hero, object, dst_index); break;


    	default: break;
    }

    if(MP2::isNeedStayFront(object))
        hero.GetPath().Reset();

    HeroesActionComplete(hero, dst_index);
}

void AIToHeroes(Heroes & hero, u32 obj, s32 dst_index)
{
    const Settings & conf = Settings::Get();
    Heroes* other_hero = world.GetTiles(dst_index).GetHeroes();
    if(! other_hero) return;

    if(hero.GetColor() == other_hero->GetColor() ||
	(conf.ExtUnionsAllowHeroesMeetings() && Players::isFriends(hero.GetColor(), other_hero->GetColor())))
    {
    	DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " meeting " << other_hero->GetName());
    	AIMeeting(hero, *other_hero);
    }
    else
    if(hero.isFriends(other_hero->GetColor()))
    {
    	DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " disable meeting");
    }
    else
    if(! hero.AllowBattle(true))
    {
        DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " currently can not allow battle");
    }
    else
    if(! other_hero->AllowBattle(false))
    {
        DEBUG(DBG_AI, DBG_INFO, other_hero->GetName() << " currently can not allow battle");
    }
    else
    {
        Castle* other_hero_castle = other_hero->inCastle();
        if(other_hero_castle && other_hero == other_hero_castle->GetHeroes().GuardFirst())
        {
            AIToCastle(hero, MP2::OBJ_CASTLE, dst_index);
            return;
        }

        //bool disable_auto_move = hero.isShipMaster() || other_hero->isShipMaster() ||
        //                    other_hero_castle || world.GetTiles(hero.GetIndex()).GetObject(false) == MP2::OBJ_STONELIGHTS;

        DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " attack enemy hero " << other_hero->GetName());

            // new battle
            Battle::Result res = Battle::Loader(hero.GetArmy(), other_hero->GetArmy(), dst_index);

            // loss defender
            if(!res.DefenderWins())
               AIBattleLose(*other_hero, res, false, hero.GetColor());

            // loss attacker
            if(!res.AttackerWins())
                AIBattleLose(hero, res, true, other_hero->GetColor());

            // wins attacker
            if(res.AttackerWins())
            {
                hero.IncreaseExperience(res.GetExperienceAttacker());

        	// disable: auto move hero for AI
		/*
        	if(conf.ExtHeroAutoMove2BattleTarget() && !disable_auto_move)
        	{
            	    hero.Move2Dest(dst_index);
        	}
		*/
            }
            else
            // wins defender
            if(res.DefenderWins())
            {
                other_hero->IncreaseExperience(res.GetExperienceDefender());
            }
    }
}

void AIToCastle(Heroes & hero, u32 obj, s32 dst_index)
{
    const Settings & conf = Settings::Get();
    Castle* castle = world.GetCastle(Maps::GetPoint(dst_index));

    if(! castle) return;

    if(hero.GetColor() == castle->GetColor() ||
	(conf.ExtUnionsAllowCastleVisiting() && Players::isFriends(hero.GetColor(), castle->GetColor())))
    {
	DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " goto castle " << castle->GetName());
	castle->MageGuildEducateHero(hero);
	hero.SetVisited(dst_index);
    }
    if(hero.isFriends(castle->GetColor()))
    {
	DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " disable visiting");
    }
    else
    {
        CastleHeroes heroes = castle->GetHeroes();

        // first attack to guest hero
        if(heroes.FullHouse())
        {
            AIToHeroes(hero, MP2::OBJ_HEROES, dst_index);
            return;
        }

        Army & army = castle->GetActualArmy();
	//bool allow_enter = false;

	if(army.isValid())
	{
    	    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " attack enemy castle " << castle->GetName());

	    Heroes* defender = heroes.GuardFirst();
	    castle->ActionPreBattle();

            // new battle
            Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);

	    castle->ActionAfterBattle(res.AttackerWins());

            // loss defender
            if(!res.DefenderWins() && defender)
               AIBattleLose(*defender, res, false, hero.GetColor());

            // loss attacker
            if(!res.AttackerWins())
                AIBattleLose(hero, res, true, castle->GetColor());

            // wins attacker
            if(res.AttackerWins())
            {
                castle->GetKingdom().RemoveCastle(castle);
                hero.GetKingdom().AddCastle(castle);
                world.CaptureObject(dst_index, hero.GetColor());
    		castle->Scoute();

                hero.IncreaseExperience(res.GetExperienceAttacker());
		//allow_enter = true;
            }
            else
            // wins defender
            if(res.DefenderWins() && defender)
            {
                defender->IncreaseExperience(res.GetExperienceDefender());
            }
	}
	else
	{
    	    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " capture enemy castle " << castle->GetName());

    	    castle->GetKingdom().RemoveCastle(castle);
	    hero.GetKingdom().AddCastle(castle);
    	    world.CaptureObject(dst_index, hero.GetColor());
    	    castle->Scoute();
	    //allow_enter = true;
	}

        // disable: auto move hero to castle for AI
	/*
        if(conf.ExtHeroAutoMove2BattleTarget() && allow_enter)
        {
            hero.Move2Dest(dst_index);
            AIToCastle(hero, MP2::OBJ_CASTLE, dst_index);
        }
	*/
    }
}

void AIToMonster(Heroes & hero, u32 obj, s32 dst_index)
{
    bool destroy = false;
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const Troop & troop = tile.QuantityTroop();
    //const Settings & conf = Settings::Get();

    JoinCount join = Army::GetJoinSolution(hero, tile);

    // free join
    if(JOIN_FREE == join.first)
    {
    	// join if ranged or flying monsters present
    	if(hero.GetArmy().HasMonster(troop()) || troop.isArchers() || troop.isFly())
    	{
    	    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " join monster " << troop.GetName());
    	    hero.GetArmy().JoinTroop(troop);
	    destroy = true;
    	}
	else
	    join.first = JOIN_NONE;
    }
    else
    // join with cost
    if(JOIN_COST == join.first)
    {
    	// join if archers or fly or present
    	if(hero.GetArmy().HasMonster(troop()) || troop.isArchers() || troop.isFly())
    	{
	    u32 gold = troop.GetCost().gold;
    	    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " join monster " << troop.GetName() << ", count: " << join.second << ", cost: " << gold);
    	    hero.GetArmy().JoinTroop(troop(), join.second);
	    hero.GetKingdom().OddFundsResource(Funds(Resource::GOLD, gold));
	    destroy = true;
    	}
	else
	    join.first = JOIN_NONE;
    }

    //bool allow_move = false;

    // fight
    if(JOIN_NONE == join.first)
    {
	DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " attack monster " << troop.GetName());

	Army army(tile);

    	Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);

    	if(res.AttackerWins())
    	{
    	    hero.IncreaseExperience(res.GetExperienceAttacker());
    	    destroy = true;
	    //allow_move = true;
    	}
    	else
    	{
    	    AIBattleLose(hero, res, true);
    	    if(Settings::Get().ExtWorldSaveMonsterBattle())
    	    {
            	tile.MonsterSetCount(army.GetCountMonsters(troop()));
            	if(tile.MonsterJoinConditionFree()) tile.MonsterSetJoinCondition(Monster::JOIN_CONDITION_MONEY);
    	    }
    	}
    }
    // unknown
    else
	destroy = true;


    if(destroy)
    {
        Maps::TilesAddon *addon = tile.FindObject(MP2::OBJ_MONSTER);
        if(addon)
        {
            const u32 uniq = addon->uniq;
            tile.Remove(uniq);
    	    tile.MonsterSetCount(0);
            tile.SetObject(MP2::OBJ_ZERO);

            // remove shadow from left cell
            if(Maps::isValidDirection(dst_index, Direction::LEFT))
                world.GetTiles(Maps::GetDirectionIndex(dst_index, Direction::LEFT)).Remove(uniq);
        }

        // auto move hero
	// disable: https://sourceforge.net/tracker/index.php?func=detail&aid=3155230&group_id=96859&atid=616180
	/*
        if(conf.ExtHeroAutoMove2BattleTarget() && allow_move)
        {
            hero.Move2Dest(dst_index);
        }
	*/
    }
}

void AIToPickupResource(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    if(obj != MP2::OBJ_BOTTLE)
    {
	hero.GetKingdom().AddFundsResource(tile.QuantityFunds());
    }

    tile.RemoveObjectSprite();
    tile.QuantityReset();
    hero.GetPath().Reset();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " pickup small resource");
}

void AIToTreasureChest(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    u32 gold = tile.QuantityGold();

    if(tile.isWater())
    {
	if(gold)
	{
	    const Artifact & art = tile.QuantityArtifact();

	    if(art.isValid() && !hero.PickupArtifact(art))
		gold = GoldInsteadArtifact(obj);
	}
    }
    else
    {
        const Artifact & art = tile.QuantityArtifact();

        if(gold)
	{
            const u32 expr = gold > 500 ? gold - 500 : 500;
	    // select gold or exp
	    if(Rand::Get(1))
	    {
		gold = 0;
		hero.IncreaseExperience(expr);
	    }
	}
	else
	if(art.isValid() && !hero.PickupArtifact(art))
	    gold = GoldInsteadArtifact(obj);
    }

    if(gold)
	hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));

    tile.RemoveObjectSprite();
    tile.QuantityReset();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToResource(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const ResourceCount & rc = tile.QuantityResourceCount();

    if(rc.isValid())
	hero.GetKingdom().AddFundsResource(Funds(rc));

    if(MP2::isCaptureObject(obj))
        AIToCaptureObject(hero, obj, dst_index);

    tile.QuantityReset();
    hero.SetVisited(dst_index, Visit::GLOBAL);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToSkeleton(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    // artifact
    if(tile.QuantityIsValid())
    {
	const Artifact & art = tile.QuantityArtifact();

	if(! hero.PickupArtifact(art))
	{
            u32 gold = GoldInsteadArtifact(obj);
            hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
        }

        tile.QuantityReset();
    }

    hero.SetVisitedWideTile(dst_index, obj, Visit::GLOBAL);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToWagon(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    if(tile.QuantityIsValid())
    {
	const Artifact & art = tile.QuantityArtifact();

        if(art.isValid())
	    hero.PickupArtifact(art);
        else
            hero.GetKingdom().AddFundsResource(tile.QuantityFunds());

        tile.QuantityReset();
    }

    hero.SetVisited(dst_index, Visit::GLOBAL);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToCaptureObject(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    // capture object
    if(! hero.isFriends(tile.QuantityColor()))
    {
	bool capture = true;

	if(tile.CaptureObjectIsProtection())
	{
	    const Troop & troop = tile.QuantityTroop();
	    Army army;
            army.JoinTroop(troop);
            army.SetColor(tile.QuantityColor());

	    Battle::Result result = Battle::Loader(hero.GetArmy(), army, dst_index);

	    if(result.AttackerWins())
	    {
		hero.IncreaseExperience(result.GetExperienceAttacker());
	    }
	    else
	    {
		capture = false;
	        AIBattleLose(hero, result, true);
		if(Settings::Get().ExtWorldSaveMonsterBattle())
		    tile.MonsterSetCount(army.GetCountMonsters(troop.GetMonster()));
	    }
	}

	if(capture)
	{
    	    // update abandone mine
    	    if(obj == MP2::OBJ_ABANDONEDMINE)
    	    {
            	tile.UpdateAbandoneMineSprite(tile);
            	hero.SetMapsObject(MP2::OBJ_MINES);
    	    }

    	    // reset spell info
    	    Maps::TilesAddon* addon = tile.FindObject(MP2::OBJ_MINES);
    	    if(addon) addon->tmp  = 0;

    	    tile.QuantitySetColor(hero.GetColor());

            if(MP2::OBJ_LIGHTHOUSE == obj)
                Maps::ClearFog(dst_index, Game::GetViewDistance(Game::VIEW_LIGHT_HOUSE), hero.GetColor());
	}
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName() << " captured: " << MP2::StringObject(obj));
}

void AIToFlotSam(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    hero.GetKingdom().AddFundsResource(tile.QuantityFunds());
    tile.RemoveObjectSprite();
    tile.QuantityReset();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToSign(Heroes & hero, u32 obj, s32 dst_index)
{
    hero.SetVisited(dst_index, Visit::LOCAL);
    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToObservationTower(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::ClearFog(dst_index, Game::GetViewDistance(Game::VIEW_OBSERVATION_TOWER), hero.GetColor());
    hero.SetVisited(dst_index, Visit::GLOBAL);
    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToMagellanMaps(Heroes & hero, u32 obj, s32 dst_index)
{
    const Funds payment(Resource::GOLD, 1000);
    Kingdom & kingdom = hero.GetKingdom();

    if(! hero.isVisited(dst_index, Visit::GLOBAL) &&
	kingdom.AllowPayment(payment))
    {
	hero.SetVisited(dst_index, Visit::GLOBAL);
	world.ActionForMagellanMaps(hero.GetColor());
	kingdom.OddFundsResource(payment);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToTeleports(Heroes & hero, s32 index_from)
{
    s32 index_to = world.NextTeleport(index_from);
    hero.ApplyPenaltyMovement();

    if(index_from == index_to)
    {
	DEBUG(DBG_AI, DBG_WARN, "action unsuccessfully...");
	return;
    }

    if(MP2::OBJ_HEROES == world.GetTiles(index_to).GetObject())
    {
        const Heroes* other_hero = world.GetTiles(index_to).GetHeroes();

        if(other_hero)
        {
            AIToHeroes(hero, MP2::OBJ_STONELIGHTS, index_to);

            // lose battle
            if(hero.isFreeman())
                return;
            else
            if(! other_hero->isFreeman())
                DEBUG(DBG_GAME, DBG_WARN, "is busy...");
        }
    }

    hero.Move2Dest(index_to, true);
    hero.GetPath().Reset();
    hero.ActionNewPosition();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToWhirlpools(Heroes & hero, s32 index_from)
{
    s32 index_to = world.NextWhirlpool(index_from);
    hero.ApplyPenaltyMovement();

    if(index_from == index_to)
    {
	DEBUG(DBG_AI, DBG_WARN, "action unsuccessfully...");
	return;
    }

    hero.Move2Dest(index_to, true);

    Troop* troop = hero.GetArmy().GetWeakestTroop();

    if(troop && Rand::Get(1) && 1 < troop->GetCount())
	troop->SetCount(Monster::GetCountFromHitPoints(troop->GetID(), troop->GetHitPoints() - troop->GetHitPoints() * Game::GetWhirlpoolPercent() / 100));

    hero.GetPath().Reset();
    hero.ActionNewPosition();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToPrimarySkillObject(Heroes & hero, u32 obj, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);

    int skill = Skill::Primary::UNKNOWN;

    switch(obj)
    {
        case MP2::OBJ_FORT:		skill = Skill::Primary::DEFENSE; break;
        case MP2::OBJ_MERCENARYCAMP:	skill = Skill::Primary::ATTACK; break;
        case MP2::OBJ_DOCTORHUT:	skill = Skill::Primary::KNOWLEDGE; break;
        case MP2::OBJ_STANDINGSTONES:	skill = Skill::Primary::POWER; break;
        case MP2::OBJ_ARENA:
	    if(Settings::Get().ExtHeroArenaCanChoiseAnySkills())
		skill = AISelectPrimarySkill(hero);
	    else
	    {
		switch(Rand::Get(1, 3))
		{
		    case 1:
		    case 2:
			skill = Rand::Get(1) ? Skill::Primary::ATTACK : Skill::Primary::DEFENSE;
			break;

		    default:
			skill = Skill::Primary::POWER;
			break;
		}
	    }
	    break;

    	default: break;
    }

    if((MP2::OBJ_ARENA == obj && !hero.isVisited(obj)) ||
       !hero.isVisited(tile))
    {
	// increase skill
	hero.IncreasePrimarySkill(skill);
	hero.SetVisited(dst_index);

        // fix double action tile
	hero.SetVisitedWideTile(dst_index, obj);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToExperienceObject(Heroes & hero, u32 obj, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);

    u32 exp = 0;

    switch(obj)
    {
        case MP2::OBJ_GAZEBO: exp = 1000; break;
    	default: break;
    }

    // check already visited
    if(! hero.isVisited(tile) && exp)
    {
	hero.SetVisited(dst_index);
	hero.IncreaseExperience(exp);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToWitchsHut(Heroes & hero, u32 obj, s32 dst_index)
{
    const Skill::Secondary & skill = world.GetTiles(dst_index).QuantitySkill();

    // check full
    if(skill.isValid() &&
	!hero.HasMaxSecondarySkill() && !hero.HasSecondarySkill(skill.Skill()))
	hero.LearnSkill(skill);

    hero.SetVisited(dst_index);
    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToShrine(Heroes & hero, u32 obj, s32 dst_index)
{
    const Spell & spell = world.GetTiles(dst_index).QuantitySpell();
    const u32 spell_level = spell.Level();

    if(spell.isValid() &&
    // check spell book
       hero.HaveSpellBook() &&
    // check present spell (skip bag artifacts)
      !hero.HaveSpell(spell, true) &&
    // check valid level spell and wisdom skill
      !(3 == spell_level && Skill::Level::NONE == hero.GetLevelSkill(Skill::Secondary::WISDOM)))
    {
	hero.AppendSpellToBook(spell);
	hero.SetVisited(dst_index);
    }
    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToGoodLuckObject(Heroes & hero, u32 obj, s32 dst_index)
{
    // check already visited
    if(!hero.isVisited(obj)) hero.SetVisited(dst_index);
    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToGoodMoraleObject(Heroes & hero, u32 obj, s32 dst_index)
{
    u32 move = 0;

    switch(obj)
    {
        case MP2::OBJ_OASIS:		move = 800; break;
        case MP2::OBJ_WATERINGHOLE:	move = 400; break;
    	default: break;
    }

    // check already visited
    if(!hero.isVisited(obj))
    {
	// modify morale
	hero.SetVisited(dst_index);
        if(move) hero.IncreaseMovePoints(move);

        // fix double action tile
	hero.SetVisitedWideTile(dst_index, obj);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToMagicWell(Heroes & hero, u32 obj, s32 dst_index)
{
    const u32 max = hero.GetMaxSpellPoints();

    if(hero.GetSpellPoints() != max &&
	// check already visited
	!hero.isVisited(MP2::OBJ_MAGICWELL))
    {
	hero.SetVisited(dst_index);
	hero.SetSpellPoints(max);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToArtesianSpring(Heroes & hero, u32 obj, s32 dst_index)
{
    const u32 max = hero.GetMaxSpellPoints();

    if(!hero.isVisited(MP2::OBJ_ARTESIANSPRING) &&
       hero.GetSpellPoints() < max * 2)
    {
	hero.SetSpellPoints(max * 2);

        if(Settings::Get().ExtWorldArtesianSpringSeparatelyVisit())
            hero.SetVisited(dst_index, Visit::LOCAL);
        else
            // fix double action tile
            hero.SetVisitedWideTile(dst_index, obj, Visit::LOCAL);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToXanadu(Heroes & hero, u32 obj, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);
    const u32 level1 = hero.GetLevelSkill(Skill::Secondary::DIPLOMACY);
    const u32 level2 = hero.GetLevel();

    if(!hero.isVisited(tile) &&
      ((level1 == Skill::Level::BASIC && 7 < level2) ||
       (level1 == Skill::Level::ADVANCED && 5 < level2) ||
       (level1 == Skill::Level::EXPERT && 3 < level2) ||
       (9 < level2)))
    {
	hero.IncreasePrimarySkill(Skill::Primary::ATTACK);
	hero.IncreasePrimarySkill(Skill::Primary::DEFENSE);
	hero.IncreasePrimarySkill(Skill::Primary::KNOWLEDGE);
	hero.IncreasePrimarySkill(Skill::Primary::POWER);
	hero.SetVisited(dst_index);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToEvent(Heroes & hero, u32 obj, s32 dst_index)
{
    // check event maps
    MapEvent* event_maps = world.GetMapEvent(dst_index);

    if(event_maps && event_maps->isAllow(hero.GetColor()) && event_maps->computer)
    {
        if(event_maps->resource.GetValidItemsCount())
    	    hero.GetKingdom().AddFundsResource(event_maps->resource);
	if(event_maps->artifact.isValid())
	    hero.PickupArtifact(event_maps->artifact);
	event_maps->SetVisited(hero.GetColor());

	if(event_maps->cancel)
	    hero.SetMapsObject(MP2::OBJ_ZERO);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToUpgradeArmyObject(Heroes & hero, u32 obj, s32 dst_index)
{
    switch(obj)
    {
	case MP2::OBJ_HILLFORT:
	    if(hero.GetArmy().HasMonster(Monster::DWARF))
		hero.GetArmy().UpgradeMonsters(Monster::DWARF);
	    if(hero.GetArmy().HasMonster(Monster::ORC))
		hero.GetArmy().UpgradeMonsters(Monster::ORC);
	    if(hero.GetArmy().HasMonster(Monster::OGRE))
		hero.GetArmy().UpgradeMonsters(Monster::OGRE);
	    break;

	case MP2::OBJ_FREEMANFOUNDRY:
	    if(hero.GetArmy().HasMonster(Monster::PIKEMAN))
		hero.GetArmy().UpgradeMonsters(Monster::PIKEMAN);
	    if(hero.GetArmy().HasMonster(Monster::SWORDSMAN))
		hero.GetArmy().UpgradeMonsters(Monster::SWORDSMAN);
	    if(hero.GetArmy().HasMonster(Monster::IRON_GOLEM))
		hero.GetArmy().UpgradeMonsters(Monster::IRON_GOLEM);
	    break;

	default: break;
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToPoorMoraleObject(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    u32  gold = tile.QuantityGold();
    bool complete = false;

    if(gold)
    {
	Army army(tile);

        Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);
        if(res.AttackerWins())
        {
    	    hero.IncreaseExperience(res.GetExperienceAttacker());
            complete = true;
            const Artifact & art = tile.QuantityArtifact();

	    if(art.isValid() && !hero.PickupArtifact(art))
    		gold = GoldInsteadArtifact(obj);

            hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, gold));
        }
        else
        {
    	    AIBattleLose(hero, res, true);
        }
    }


    if(complete)
	tile.QuantityReset();
    else
    if(0 == gold && !hero.isVisited(obj))
    {
	// modify morale
	hero.SetVisited(dst_index);
        hero.SetVisited(dst_index, Visit::GLOBAL);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToPoorLuckObject(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const Spell & spell = tile.QuantitySpell();

    if(spell.isValid())
    {
        // battle
        Army army(tile);

	Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);

    	if(res.AttackerWins())
	{
    	    hero.IncreaseExperience(res.GetExperienceAttacker());

	    // check magick book
	    if(hero.HaveSpellBook() &&
	    // check skill level for wisdom
		Skill::Level::EXPERT == hero.GetLevelSkill(Skill::Secondary::WISDOM))
	    {
		hero.AppendSpellToBook(spell);
	    }

	    tile.QuantityReset();
	    hero.SetVisited(dst_index, Visit::GLOBAL);
	}
	else
	{
	    AIBattleLose(hero, res, true);
	}
    }
    else
    {
	hero.SetVisited(dst_index, Visit::LOCAL);
	hero.SetVisited(dst_index, Visit::GLOBAL);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToObelisk(Heroes & hero, u32 obj, s32 dst_index)
{
    if(!hero.isVisited(obj, Visit::GLOBAL))
    {
        hero.SetVisited(dst_index, Visit::GLOBAL);
        Kingdom & kingdom = hero.GetKingdom();
        kingdom.PuzzleMaps().Update(kingdom.CountVisitedObjects(MP2::OBJ_OBELISK), world.CountObeliskOnMaps());
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToTreeKnowledge(Heroes & hero, u32 obj, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);

    if(!hero.isVisited(tile))
    {
	const Funds & funds = tile.QuantityFunds();

    	if(0 == funds.GetValidItemsCount() ||
	    hero.GetKingdom().AllowPayment(funds))
    	{
	    if(funds.GetValidItemsCount())
		hero.GetKingdom().OddFundsResource(funds);
	    hero.SetVisited(dst_index);
	    hero.IncreaseExperience(hero.GetExperienceFromLevel(hero.GetLevel()) - hero.GetExperience());
	}
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}


void AIToDaemonCave(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    if(tile.QuantityIsValid())
    {
        Army army(tile);

        Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);
        if(res.AttackerWins())
	{
            hero.IncreaseExperience(res.GetExperienceAttacker());
            hero.GetKingdom().AddFundsResource(tile.QuantityFunds());
        }
        else
        {
            AIBattleLose(hero, res, true);
        }

	tile.QuantityReset();
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToDwellingJoinMonster(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const Troop & troop = tile.QuantityTroop();

    if(troop.isValid() && hero.GetArmy().JoinTroop(troop)) tile.MonsterSetCount(0);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToDwellingRecruitMonster(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const Troop & troop = tile.QuantityTroop();

    if(troop.isValid())
    {
        Kingdom & kingdom = hero.GetKingdom();
	const payment_t paymentCosts = troop.GetCost();

        if(kingdom.AllowPayment(paymentCosts) && hero.GetArmy().JoinTroop(troop))
        {
    	    tile.MonsterSetCount(0);
	    kingdom.OddFundsResource(paymentCosts);

	    // remove ancient lamp sprite
	    if(MP2::OBJ_ANCIENTLAMP == obj)
	    {
	        tile.RemoveObjectSprite();
	        tile.SetObject(MP2::OBJ_ZERO);
	    }
	}
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToStables(Heroes & hero, u32 obj, s32 dst_index)
{
    // check already visited
    if(!hero.isVisited(obj))
    {
        hero.SetVisited(dst_index);
        hero.IncreaseMovePoints(400);
    }

    if(hero.GetArmy().HasMonster(Monster::CAVALRY)) hero.GetArmy().UpgradeMonsters(Monster::CAVALRY);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToAbandoneMine(Heroes & hero, u32 obj, s32 dst_index)
{
    AIToCaptureObject(hero, obj, dst_index);
}

void AIToBarrier(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    Kingdom & kingdom = hero.GetKingdom();

    if(kingdom.IsVisitTravelersTent(tile.QuantityColor()))
    {
        tile.RemoveObjectSprite();
        tile.SetObject(MP2::OBJ_ZERO);
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToTravellersTent(Heroes & hero, u32 obj, s32 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);
    Kingdom & kingdom = hero.GetKingdom();

    kingdom.SetVisitTravelersTent(tile.QuantityColor());

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToShipwreckSurvivor(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    if(hero.IsFullBagArtifacts())
        hero.GetKingdom().AddFundsResource(Funds(Resource::GOLD, GoldInsteadArtifact(obj)));
    else
	hero.PickupArtifact(tile.QuantityArtifact());

    tile.RemoveObjectSprite();
    tile.QuantityReset();

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToArtifact(Heroes & hero, u32 obj, s32 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    if(! hero.IsFullBagArtifacts())
    {
	const u32 cond = tile.QuantityVariant();
	const Artifact & art = tile.QuantityArtifact();

        bool result = false;

	// 1,2,3 - gold, gold + res
	if(0 < cond && cond < 4)
        {
    	    const Funds & payment = tile.QuantityFunds();

    	    if(hero.GetKingdom().AllowPayment(payment))
            {
		result = true;
		hero.GetKingdom().OddFundsResource(payment);
            }
        }
	else
        // 4,5 - need have skill wisard or leadership,
	if(3 < cond && cond < 6)
	{
    	    if(hero.HasSecondarySkill(tile.QuantitySkill().Skill()))
        	result = true;
        }
	else
        // 6 - 50 rogues, 7 - 1 gin, 8,9,10,11,12,13 - 1 monster level4
	if(5 < cond && cond < 14)
        {
    	    Army army(tile);

	    // new battle
            Battle::Result res = Battle::Loader(hero.GetArmy(), army, dst_index);
            if(res.AttackerWins())
            {
        	hero.IncreaseExperience(res.GetExperienceAttacker());
                result = true;
            }
	    else
            {
        	AIBattleLose(hero, res, true);
            }
        }
        else
	{
            result = true;
        }

        if(result && hero.PickupArtifact(art))
        {
	    tile.RemoveObjectSprite();
            tile.QuantityReset();
        }
    }

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToBoat(Heroes & hero, u32 obj, s32 dst_index)
{
    if(hero.isShipMaster()) return;

    const s32 from_index = hero.GetIndex();

    // disabled nearest coasts (on week MP2::isWeekLife)
    MapsIndexes coasts = Maps::ScanAroundObject(from_index, 4, MP2::OBJ_COAST);
    coasts.push_back(from_index);

    for(MapsIndexes::const_iterator
	it = coasts.begin(); it != coasts.end(); ++it) hero.SetVisited(*it);

    hero.ResetMovePoints();
    hero.Move2Dest(dst_index);
    hero.SetMapsObject(MP2::OBJ_ZERO);
    hero.SetShipMaster(true);
    hero.GetPath().Reset();

    AI::HeroesClearTask(hero);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIToCoast(Heroes & hero, u32 obj, s32 dst_index)
{
    if(! hero.isShipMaster()) return;

    Maps::Tiles & from = world.GetTiles(hero.GetIndex());

    hero.ResetMovePoints();
    hero.Move2Dest(dst_index);
    from.SetObject(MP2::OBJ_BOAT);
    hero.SetShipMaster(false);
    hero.GetPath().Reset();

    AI::HeroesClearTask(hero);

    DEBUG(DBG_AI, DBG_INFO, hero.GetName());
}

void AIMeeting(Heroes & hero1, Heroes & hero2)
{
    if(Settings::Get().ExtWorldEyeEagleAsScholar())
        Heroes::ScholarAction(hero1, hero2);

    if(hero1.Modes(AI::HEROES_HUNTER))
	hero1.GetArmy().JoinStrongestFromArmy(hero2.GetArmy());
    else
    if(hero2.Modes(AI::HEROES_HUNTER))
	hero2.GetArmy().JoinStrongestFromArmy(hero1.GetArmy());
    else
    if(hero1.Modes(AI::HEROES_SCOUTER))
	hero1.GetArmy().KeepOnlyWeakestTroops(hero2.GetArmy());
    else
    if(hero2.Modes(AI::HEROES_SCOUTER))
	hero2.GetArmy().KeepOnlyWeakestTroops(hero1.GetArmy());

    // artifacts change
}

bool AI::HeroesValidObject(const Heroes & hero, s32 index)
{
    Maps::Tiles & tile = world.GetTiles(index);
    const u32 obj = tile.GetObject();
    const Army & army = hero.GetArmy();
    const Kingdom & kingdom = hero.GetKingdom();

    // check other
    switch(obj)
    {
	// water object
	case MP2::OBJ_SHIPWRECKSURVIROR:
	case MP2::OBJ_WATERCHEST:
	case MP2::OBJ_FLOTSAM:
	case MP2::OBJ_BOTTLE:
	    if(hero.isShipMaster()) return true;
            break;

	case MP2::OBJ_BUOY:
	    if(! hero.isVisited(obj) &&
		Morale::BLOOD > hero.GetMorale()) return true;
	    break;

	case MP2::OBJ_MERMAID:
	    if(! hero.isVisited(obj) &&
		Luck::IRISH > hero.GetLuck()) return true;
	    break;

	case MP2::OBJ_SIRENS:
		return false;

	case MP2::OBJ_MAGELLANMAPS:
	case MP2::OBJ_WHIRLPOOL:
	case MP2::OBJ_COAST:
	    if(hero.isShipMaster()) return true;
            break;

	// capture objects
	case MP2::OBJ_SAWMILL:
	case MP2::OBJ_MINES:
	case MP2::OBJ_ALCHEMYLAB:
	    if(! hero.isFriends(tile.QuantityColor()))
	    {
		if(tile.CaptureObjectIsProtection())
		{
		    Army enemy(tile);
		    return !enemy.isValid() || Army::TroopsStrongerEnemyTroops(army, enemy);
		}
		else
		return true;
	    }
	    break;

	// pickup object
	case MP2::OBJ_WAGON:
	case MP2::OBJ_LEANTO:
	case MP2::OBJ_MAGICGARDEN:
	case MP2::OBJ_SKELETON:
	    if(tile.QuantityIsValid()) return true;
	    break;

	case MP2::OBJ_WATERWHEEL:
	case MP2::OBJ_WINDMILL:
	    if(Settings::Get().ExtWorldExtObjectsCaptured())
	    {
		if(! hero.isFriends(tile.QuantityColor()))
		{
		    if(tile.CaptureObjectIsProtection())
		    {
			Army enemy(tile);
			return !enemy.isValid() || Army::TroopsStrongerEnemyTroops(army, enemy);
		    }
		    else
		    return true;
		}
	    }
	    else
	    if(tile.QuantityIsValid()) return true;
	    break;

	// pickup resource
	case MP2::OBJ_RESOURCE:
	case MP2::OBJ_CAMPFIRE:
	case MP2::OBJ_TREASURECHEST:
	    if(! hero.isShipMaster()) return true;
	    break;

	case MP2::OBJ_ARTIFACT:
	{
	    const u32 variants = tile.QuantityVariant();

	    if(hero.IsFullBagArtifacts()) return false;

	    if(hero.isShipMaster()) return false;
	    else
	    // 1,2,3 - 2000g, 2500g+3res, 3000g+5res
	    if(1 <= variants && 3 >= variants)
	    {
		return kingdom.AllowPayment(tile.QuantityFunds());
	    }
	    else
	    // 4,5 - need have skill wisard or leadership,
	    if(3 < variants && 6 > variants)
	    {
		return hero.HasSecondarySkill(tile.QuantitySkill().Skill());
	    }
	    else
	    // 6 - 50 rogues, 7 - 1 gin, 8,9,10,11,12,13 - 1 monster level4
	    if(5 < variants && 14 > variants)
	    {
                Army enemy(tile);
		return !enemy.isValid() || Army::TroopsStrongerEnemyTroops(army, enemy);
	    }
	    else
	    // other
	    return true;
	}
	break;

	// increase view
	case MP2::OBJ_OBSERVATIONTOWER:
	// obelisk
	case MP2::OBJ_OBELISK:
	    if(! hero.isVisited(tile, Visit::GLOBAL)) return true;
	    break;

        case MP2::OBJ_BARRIER:
	    if(kingdom.IsVisitTravelersTent(tile.QuantityColor())) return true;
	    break;

        case MP2::OBJ_TRAVELLERTENT:
	    if(! kingdom.IsVisitTravelersTent(tile.QuantityColor())) return true;
	    break;

	// new spell
        case MP2::OBJ_SHRINE1:
	case MP2::OBJ_SHRINE2:
	case MP2::OBJ_SHRINE3:
	{
	    const Spell & spell = tile.QuantitySpell();
	    if(spell.isValid() &&
		 // check spell book
		hero.HaveSpellBook() &&
		!hero.HaveSpell(spell) &&
                // check valid level spell and wisdom skill
                !(3 == spell.Level() &&
                Skill::Level::NONE == hero.GetLevelSkill(Skill::Secondary::WISDOM))) return true;
	}
	    break;

    	// primary skill
	case MP2::OBJ_FORT:
    	case MP2::OBJ_MERCENARYCAMP:
    	case MP2::OBJ_DOCTORHUT:
    	case MP2::OBJ_STANDINGSTONES:
	// sec skill
	case MP2::OBJ_WITCHSHUT:
	// exp
	case MP2::OBJ_GAZEBO:
	    if(! hero.isVisited(tile)) return true;
	    break;

	case MP2::OBJ_TREEKNOWLEDGE:
	    if(! hero.isVisited(tile))
	    {
		const ResourceCount & rc = tile.QuantityResourceCount();
    		if(! rc.isValid() || kingdom.AllowPayment(Funds(rc)))
		    return true;
	    }
	    break;

    	// good luck
        case MP2::OBJ_FOUNTAIN:
    	case MP2::OBJ_FAERIERING:
    	case MP2::OBJ_IDOL:
	    if(! hero.isVisited(obj) &&
		Luck::IRISH > hero.GetLuck()) return true;
	    break;

	// good morale
	case MP2::OBJ_OASIS:
    	case MP2::OBJ_TEMPLE:
    	case MP2::OBJ_WATERINGHOLE:
	    if(! hero.isVisited(obj) &&
		Morale::BLOOD > hero.GetMorale()) return true;
	    break;

	case MP2::OBJ_MAGICWELL:
	    if(! hero.isVisited(tile) &&
		hero.GetMaxSpellPoints() != hero.GetSpellPoints()) return true;
	    break;

	case MP2::OBJ_ARTESIANSPRING:
	    if(! hero.isVisited(tile) &&
		2 * hero.GetMaxSpellPoints() > hero.GetSpellPoints()) return true;
	    break;

	case MP2::OBJ_XANADU:
	{
	    const u32 level1 = hero.GetLevelSkill(Skill::Secondary::DIPLOMACY);
	    const u32 level2 = hero.GetLevel();

	    if(!hero.isVisited(tile) &&
		((level1 == Skill::Level::BASIC && 7 < level2) ||
		(level1 == Skill::Level::ADVANCED && 5 < level2) ||
		(level1 == Skill::Level::EXPERT && 3 < level2) || (9 < level2))) return true;
	    break;
	}

        // accept army
        case MP2::OBJ_WATCHTOWER:
        case MP2::OBJ_EXCAVATION:
        case MP2::OBJ_CAVE:
        case MP2::OBJ_TREEHOUSE:
        case MP2::OBJ_ARCHERHOUSE:
        case MP2::OBJ_GOBLINHUT:
        case MP2::OBJ_DWARFCOTT:
	case MP2::OBJ_HALFLINGHOLE:
        case MP2::OBJ_PEASANTHUT:
        case MP2::OBJ_THATCHEDHUT:
        {
    	    const Troop & troop = tile.QuantityTroop();
	    if(troop.isValid() &&
		(army.HasMonster(troop()) ||
		(! army.isFullHouse() && (troop.isArchers() || troop.isFly())))) return true;
	    break;
	}

        // recruit army
        case MP2::OBJ_RUINS:
        case MP2::OBJ_TREECITY:
        case MP2::OBJ_WAGONCAMP:
	case MP2::OBJ_DESERTTENT:
        case MP2::OBJ_WATERALTAR:
        case MP2::OBJ_AIRALTAR:
        case MP2::OBJ_FIREALTAR:
        case MP2::OBJ_EARTHALTAR:
        case MP2::OBJ_BARROWMOUNDS:
	{
	    const Troop & troop = tile.QuantityTroop();
	    const payment_t paymentCosts = troop.GetCost();

	    if(troop.isValid() && kingdom.AllowPayment(paymentCosts) &&
		(army.HasMonster(troop()) ||
		(! army.isFullHouse() && (troop.isArchers() || troop.isFly())))) return true;
	    break;
	}

        // recruit army (battle)
        case MP2::OBJ_DRAGONCITY:
        case MP2::OBJ_CITYDEAD:
        case MP2::OBJ_TROLLBRIDGE:
        {
    	    const bool battle = (Color::NONE == tile.QuantityColor());
	    if(!battle)
	    {
		const Troop & troop = tile.QuantityTroop();
		const payment_t paymentCosts = troop.GetCost();

		if(troop.isValid() && kingdom.AllowPayment(paymentCosts) &&
		(army.HasMonster(troop()) ||
		(! army.isFullHouse()))) return true;
	    }
	    break;
        }

	// recruit genie
	case MP2::OBJ_ANCIENTLAMP:
	{
	    const Troop & troop = tile.QuantityTroop();
	    const payment_t paymentCosts = troop.GetCost();

	    if(troop.isValid() && kingdom.AllowPayment(paymentCosts) &&
		(army.HasMonster(troop()) ||
		(! army.isFullHouse()))) return true;
	    break;
	}

	// upgrade army
	case MP2::OBJ_HILLFORT:
            if(army.HasMonster(Monster::DWARF) ||
               army.HasMonster(Monster::ORC) ||
               army.HasMonster(Monster::OGRE)) return true;
            break;

	// upgrade army
        case MP2::OBJ_FREEMANFOUNDRY:
            if(army.HasMonster(Monster::PIKEMAN) ||
               army.HasMonster(Monster::SWORDSMAN) ||
               army.HasMonster(Monster::IRON_GOLEM)) return true;
            break;

	// loyalty obj
	case MP2::OBJ_STABLES:
	    if(army.HasMonster(Monster::CAVALRY) ||
		! hero.isVisited(tile)) return true;
	    break;

	case MP2::OBJ_ARENA:
	    if(! hero.isVisited(tile)) return true;
	    break;

	// poor morale obj
	case MP2::OBJ_SHIPWRECK:
        case MP2::OBJ_GRAVEYARD:
	case MP2::OBJ_DERELICTSHIP:
	    if(! hero.isVisited(tile, Visit::GLOBAL) &&
		tile.QuantityIsValid())
	    {
		Army enemy(tile);
		return enemy.isValid() && Army::TroopsStrongerEnemyTroops(army, enemy);
	    }
	    break;

	//case MP2::OBJ_PYRAMID:

	case MP2::OBJ_DAEMONCAVE:
	    if(tile.QuantityIsValid() && 4 != tile.QuantityVariant()) return true;
	    break;

	case MP2::OBJ_MONSTER:
	{
	    Army enemy(tile);
	    return !enemy.isValid() || Army::TroopsStrongerEnemyTroops(army, enemy);
	}
	break;

	// sign
	case MP2::OBJ_SIGN:
	    if(!hero.isVisited(tile)) return true;
	    break;

	case MP2::OBJ_CASTLE:
	{
	    const Castle* castle = world.GetCastle(Maps::GetPoint(index));
	    if(castle)
	    {
		if(hero.GetColor() == castle->GetColor())
		    return NULL == castle->GetHeroes().Guest() && ! hero.isVisited(tile);
		else
		// FIXME: AI skip visiting alliance
		if(hero.isFriends(castle->GetColor())) return false;
		else
		if(Army::TroopsStrongerEnemyTroops(army, castle->GetActualArmy())) return true;
	    }
	    break;
	}

	case MP2::OBJ_HEROES:
	{
	    const Heroes* hero2 = tile.GetHeroes();
	    if(hero2)
	    {
		if(hero.GetColor() == hero2->GetColor()) return true;
		// FIXME: AI skip visiting alliance
		else
		if(hero.isFriends(hero2->GetColor())) return false;
		else
		if(hero2->AllowBattle(false) &&
		    Army::TroopsStrongerEnemyTroops(army, hero2->GetArmy())) return true;
	    }
	    break;
	}

	case MP2::OBJ_BOAT:
	case MP2::OBJ_STONELIGHTS:
	    // check later
	    return true;

	// or add later

	default: break;
    }

    return false;
}

bool AIHeroesShowAnimation(const Heroes & hero)
{
    const Settings & conf = Settings::Get();

    // accumulate colors
    u32 colors = 0;

    if(conf.GameType() & Game::TYPE_HOTSEAT)
    {
        const Colors vcolors(Players::HumanColors());

        for(Colors::const_iterator
            it = vcolors.begin(); it != vcolors.end(); ++it)
        {
            const Player* player = conf.GetPlayers().Get(*it);
            if(player) colors |= player->friends;
        }
    }
    else
    {
        const Player* player = conf.GetPlayers().Get(Players::HumanColors());
        if(player) colors = player->friends;
    }

    // get result
    const s32 index_from = hero.GetIndex();

    if(colors && Maps::isValidAbsIndex(index_from))
    {
        const Maps::Tiles & tile_from = world.GetTiles(index_from);

        if(hero.GetPath().isValid())
        {
            const s32 index_to = Maps::GetDirectionIndex(index_from, hero.GetPath().GetFrontDirection());
            const Maps::Tiles & tile_to = world.GetTiles(index_to);

            return !tile_from.isFog(colors) && !tile_to.isFog(colors);
        }

        return !tile_from.isFog(colors);
    }

    return false;
}

void AI::HeroesMove(Heroes & hero)
{
    if(hero.GetPath().isValid())
    {
	hero.SetMove(true);

	if(1)
	{
	    const Settings & conf = Settings::Get();
	    Display & display = Display::Get();
	    Cursor & cursor = Cursor::Get();
	    Interface::Basic & I = Interface::Basic::Get();

	    cursor.Hide();

	    if(0 != conf.AIMoveSpeed() && AIHeroesShowAnimation(hero))
	    {
		cursor.Hide();
		I.GetGameArea().SetCenter(hero.GetCenter());
		I.Redraw(REDRAW_GAMEAREA);
		cursor.Show();
		display.Flip();
	    }

	    while(LocalEvent::Get().HandleEvents())
	    {
		if(hero.isFreeman() || !hero.isEnableMove()) break;

		bool hide_move = (0 == conf.AIMoveSpeed()) ||
		    (! IS_DEVEL() && !AIHeroesShowAnimation(hero));

		if(hide_move)
		{
		    hero.Move(true);
		}
		else
		if(Game::AnimateInfrequentDelay(Game::CURRENT_AI_DELAY))
		{
		    cursor.Hide();
		    hero.Move();

		    I.GetGameArea().SetCenter(hero.GetCenter());
		    I.Redraw(REDRAW_GAMEAREA);
		    cursor.Show();
		    display.Flip();
		}

    		if(Game::AnimateInfrequentDelay(Game::MAPS_DELAY))
		{
		    u32 & frame = Game::MapsAnimationFrame();
		    ++frame;
		    cursor.Hide();
		    I.Redraw(REDRAW_GAMEAREA);
		    cursor.Show();
		    display.Flip();
		}
	    }

	    bool hide_move = (0 == conf.AIMoveSpeed()) ||
		(! IS_DEVEL() && !AIHeroesShowAnimation(hero));

	    // 0.2 sec delay for show enemy hero position
	    if(!hero.isFreeman() && !hide_move) DELAY(200);
	}
    }
}
