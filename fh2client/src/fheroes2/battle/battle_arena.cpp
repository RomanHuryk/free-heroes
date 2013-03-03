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

#include <algorithm>
#include "settings.h"
#include "network_protocol.h"
#include "tools.h"
#include "army.h"
#include "cursor.h"
#include "castle.h"
#include "world.h"
#include "agg.h"
#include "speed.h"
#include "race.h"
#include "army_troop.h"
#include "spell_storage.h"
#include "battle_arena.h"
#include "battle_cell.h"
#include "battle_army.h"
#include "battle_troop.h"
#include "battle_tower.h"
#include "battle_catapult.h"
#include "battle_bridge.h"
#include "battle_command.h"
#include "battle_interface.h"

namespace Battle
{
    Arena* arena = NULL;
}

ICN::icn_t GetCovr(u16 ground)
{
    std::vector<ICN::icn_t> covrs;

    switch(ground)
    {
        case Maps::Ground::SNOW:
            covrs.push_back(ICN::COVR0007);
            covrs.push_back(ICN::COVR0008);
            covrs.push_back(ICN::COVR0009);
            covrs.push_back(ICN::COVR0010);
            covrs.push_back(ICN::COVR0011);
            covrs.push_back(ICN::COVR0012);
            break;

        case Maps::Ground::WASTELAND:
            covrs.push_back(ICN::COVR0019);
            covrs.push_back(ICN::COVR0020);
            covrs.push_back(ICN::COVR0021);
            covrs.push_back(ICN::COVR0022);
            covrs.push_back(ICN::COVR0023);
            covrs.push_back(ICN::COVR0024);
            break;

        case Maps::Ground::DIRT:
            covrs.push_back(ICN::COVR0013);
            covrs.push_back(ICN::COVR0014);
            covrs.push_back(ICN::COVR0015);
            covrs.push_back(ICN::COVR0016);
            covrs.push_back(ICN::COVR0017);
            covrs.push_back(ICN::COVR0018);
            break;

        case Maps::Ground::GRASS:
            covrs.push_back(ICN::COVR0001);
            covrs.push_back(ICN::COVR0002);
            covrs.push_back(ICN::COVR0003);
            covrs.push_back(ICN::COVR0004);
            covrs.push_back(ICN::COVR0005);
            covrs.push_back(ICN::COVR0006);
            break;

        default: break;
    }

    return covrs.empty() ? ICN::UNKNOWN : *Rand::Get(covrs);
}

StreamBase & Battle::operator<< (StreamBase & msg, const TargetInfo & t)
{
    return msg <<
	(t.defender ? t.defender->GetUID() : static_cast<u32>(0)) <<
	t.damage << t.killed << t.resist;
}

StreamBase & Battle::operator>> (StreamBase & msg, TargetInfo & t)
{
    u32 uid = 0;

    msg >> uid >>
	t.damage >> t.killed >> t.resist;

    t.defender = uid ? GetArena()->GetTroopUID(uid) : NULL;

    return msg;
}

StreamBase & Battle::operator<< (StreamBase & msg, const TargetsInfo & ts)
{
    msg << static_cast<u32>(ts.size());

    for(TargetsInfo::const_iterator
	it = ts.begin(); it != ts.end(); ++it)
	msg << *it;

    return msg;
}

StreamBase & Battle::operator>> (StreamBase & msg, TargetsInfo & ts)
{
    u32 size = 0;

    msg >> size;
    ts.resize(size);

    for(TargetsInfo::iterator
	it = ts.begin(); it != ts.end(); ++it)
	msg >> *it;

    return msg;
}

bool Battle::TargetInfo::operator==(const TargetInfo & ta) const
{
    return defender == ta.defender;
}

Battle::Arena* Battle::GetArena(void)
{
    return arena;
}

const Castle* Battle::Arena::GetCastle(void)
{
    return arena->castle;
}

Battle::Bridge* Battle::Arena::GetBridge(void)
{
    return arena->bridge;
}

Battle::Board* Battle::Arena::GetBoard(void)
{
    return &arena->board;
}

Battle::Graveyard* Battle::Arena::GetGraveyard(void)
{
    return &arena->graveyard;
}

Battle::Interface* Battle::Arena::GetInterface(void)
{
    return arena->interface;
}

Battle::Tower* Battle::Arena::GetTower(u8 type)
{
    switch(type)
    {
        case TWR_LEFT:  return arena->towers[0];
        case TWR_CENTER:return arena->towers[1];
        case TWR_RIGHT: return arena->towers[2];
        default: break;
    }
    return NULL;
}

Battle::Arena::Arena(Army & a1, Army & a2, s32 index, bool local) :
	army1(NULL), army2(NULL), armies_order(NULL), castle(NULL), current_color(0), catapult(NULL),
	bridge(NULL), interface(NULL), icn_covr(ICN::UNKNOWN), current_turn(0), auto_battle(0), end_turn(false)
{
    const Settings & conf = Settings::Get();
    usage_spells.reserve(20);

    arena = this;
    army1 = new Force(a1, false);
    army2 = new Force(a2, true);

    // init castle (interface ahead)
    castle = world.GetCastle(index);

    if(castle)
    {
	CastleHeroes heroes = world.GetHeroes(*castle);

	// skip if present guard and guest
	if(heroes.FullHouse()) castle = NULL;

	// skip for town
	if(castle && !castle->isCastle()) castle = NULL;
    }

    // init interface
    if(local)
    {
	interface = new Interface(*this, index);
	board.SetArea(interface->GetArea());

	std::for_each(army1->begin(), army1->end(), std::mem_fun(&Unit::InitContours));
	std::for_each(army2->begin(), army2->end(), std::mem_fun(&Unit::InitContours));

	if(conf.Sound())
	    AGG::PlaySound(M82::PREBATTL);

	armies_order = new Units();
	armies_order->reserve(25);
	interface->SetArmiesOrder(armies_order);
    }


    towers[0] = NULL;
    towers[1] = NULL;
    towers[2] = NULL;

    if(castle)
    {
	// init
	towers[0] = castle->isBuild(BUILD_LEFTTURRET) ? new Tower(*castle, TWR_LEFT) : NULL;
	towers[1] = new Tower(*castle, TWR_CENTER);
	towers[2] = castle->isBuild(BUILD_RIGHTTURRET) ? new Tower(*castle, TWR_RIGHT) : NULL;
	bool fortification = (Race::KNGT == castle->GetRace()) && castle->isBuild(BUILD_SPEC);
	catapult = army1->GetCommander() ? new Catapult(*army1->GetCommander(), fortification) : NULL;
	bridge = new Bridge();

	// catapult cell
	board[77].SetObject(1);

	// wall (3,2,1,0)
	board[8].SetObject(fortification ? 3 : 2);
	board[29].SetObject(fortification ? 3 : 2);
	board[73].SetObject(fortification ? 3 : 2);
	board[96].SetObject(fortification ? 3 : 2);

	// tower
	board[40].SetObject(2);
	board[62].SetObject(2);

	// archers tower
	board[19].SetObject(2);
        board[85].SetObject(2);

	// bridge
	board[49].SetObject(1);
	board[50].SetObject(1);
    }
    else
    // set obstacles
    {
	icn_covr = Maps::ScanAroundObject(index, MP2::OBJ_CRATER).size() ?
			GetCovr(world.GetTiles(index).GetGround()) : ICN::UNKNOWN;

	if(icn_covr != ICN::UNKNOWN)
	    board.SetCovrObjects(icn_covr);
	else
	    board.SetCobjObjects(world.GetTiles(index));
    }

    // set guardian objects mode (+2 defense)
    if(conf.ExtWorldGuardianObjectsTwoDefense() &&
	!castle &&
        MP2::isCaptureObject(world.GetTiles(index).GetObject(false)))
        army2->SetModes(ARMY_GUARDIANS_OBJECT);

    //
    if(interface)
    {
	Cursor & cursor = Cursor::Get();
	Display & display = Display::Get();

	cursor.Hide();
	cursor.SetThemes(Cursor::WAR_NONE);

	if(conf.ExtGameUseFade()) display.Fade();

	interface->Redraw();
	cursor.Show();
	display.Flip();

	// pause for play M82::PREBATTL
	if(conf.Sound())
	    while(LocalEvent::Get().HandleEvents() && Mixer::isPlaying(-1));
    }
}

Battle::Arena::~Arena()
{
    delete army1;
    delete army2;

    if(towers[0]) delete towers[0];
    if(towers[1]) delete towers[1];
    if(towers[2]) delete towers[2];

    if(catapult) delete catapult;
    if(interface) delete interface;
    if(armies_order) delete armies_order;
}

void Battle::Arena::TurnTroop(Unit* current_troop)
{
    Actions actions;
    end_turn = false;

    DEBUG(DBG_BATTLE, DBG_TRACE, current_troop->String(true));

    while(! end_turn)
    {
	// bad morale
	if(current_troop->Modes(MORALE_BAD))
	{
    	    actions.push_back(Command(MSG_BATTLE_MORALE, current_troop->GetUID(), false));
	    end_turn = true;
	}
	else
	{
	    // turn opponents
	    if(CONTROL_REMOTE & current_troop->GetControl())
		RemoteTurn(*current_troop, actions);
	    else
	    {
		if((CONTROL_AI & current_troop->GetControl()) ||
		    (current_color & auto_battle))
		    {
			AI::BattleTurn(*this, *current_troop, actions);
		    }
    		else
		if(CONTROL_HUMAN & current_troop->GetControl())
		    HumanTurn(*current_troop, actions);
	    }
	}

	// apply task
	while(actions.size())
	{
	    // apply action
	    ApplyAction(actions.front().GetStream());
	    actions.pop_front();

	    // rescan orders
	    if(armies_order) Force::UpdateOrderUnits(*army1, *army2, *armies_order);

    	    // check end battle
    	    if(! BattleValid())
		end_turn = true;

	    // good morale
	    if(end_turn &&
		    !current_troop->Modes(TR_SKIPMOVE) &&
		    current_troop->Modes(TR_MOVED) &&
		    current_troop->Modes(MORALE_GOOD) &&
		    BattleValid())
	    {
		actions.push_back(Command(MSG_BATTLE_MORALE, current_troop->GetUID(), true));
		end_turn = false;
	    }
	}

	if(current_troop->Modes(TR_SKIPMOVE | TR_MOVED))
	    end_turn = true;

	board.Reset();

	DELAY(10);
    }
}

bool Battle::Arena::BattleValid(void) const
{
    return army1->isValid() && army2->isValid() &&
	0 == result_game.army1 && 0 == result_game.army2;
}

void Battle::Arena::Turns(void)
{
    const Settings & conf = Settings::Get();

    ++current_turn;
    DEBUG(DBG_BATTLE, DBG_TRACE, current_turn);

    if(interface && conf.Music() && !Music::isPlaying())
            AGG::PlayMusic(MUS::GetBattleRandom(), false);

    army1->NewTurn();
    army2->NewTurn();

    bool tower_moved = false;
    bool catapult_moved = false;

    Unit* current_troop = NULL;

    // rescan orders
    if(armies_order) Force::UpdateOrderUnits(*army1, *army2, *armies_order);

    while(BattleValid() &&
	NULL != (current_troop = Force::GetCurrentUnit(*army1, *army2, current_troop, true)))
    {
	current_color = current_troop->GetArmyColor();

	// first turn: castle and catapult action
	if(castle)
	{
	    if(!catapult_moved && current_troop->GetColor() == army1->GetColor())
	    {
		CatapultAction();
		catapult_moved = true;
	    }

	    if(!tower_moved && current_troop->GetColor() == army2->GetColor())
	    {
		if(towers[0] && towers[0]->isValid()) TowerAction(*towers[0]);
		if(towers[1] && towers[1]->isValid()) TowerAction(*towers[1]);
		if(towers[2] && towers[2]->isValid()) TowerAction(*towers[2]);
		tower_moved = true;

    		// check dead last army from towers
    		if(! BattleValid()) break;
	    }
	}

	// set bridge passable
	if(bridge && bridge->isValid() && !bridge->isDown()) bridge->SetPassable(*current_troop);

	// turn troop
	TurnTroop(current_troop);
    }

    current_troop = NULL;

    // can skip move ?
    if(Settings::Get().ExtBattleSoftWait())
    while(BattleValid() &&
	NULL != (current_troop = Force::GetCurrentUnit(*army1, *army2, current_troop, false)))
    {
	current_color = current_troop->GetArmyColor();

	// set bridge passable
	if(bridge && bridge->isValid() && !bridge->isDown()) bridge->SetPassable(*current_troop);

	// turn troop
	TurnTroop(current_troop);
    }

    // end turn: fix result
    if(!army1->isValid() || (result_game.army1 & (RESULT_RETREAT | RESULT_SURRENDER)))
    {
	result_game.army1 |= RESULT_LOSS;
	if(army2->isValid()) result_game.army2 = RESULT_WINS;
    }

    if(!army2->isValid() || (result_game.army2 & (RESULT_RETREAT | RESULT_SURRENDER)))
    {
	result_game.army2 |= RESULT_LOSS;
	if(army1->isValid()) result_game.army1 = RESULT_WINS;
    }

    // fix experience and killed
    if(result_game.army1 || result_game.army2)
    {
        result_game.exp1 = army2->GetDeadHitPoints();
        result_game.exp2 = army1->GetDeadHitPoints();

        if(army1->GetCommander()) result_game.exp2 += 500;
        if(army2->GetCommander()) result_game.exp1 += 500;

	Force *army_loss = (result_game.army1 & RESULT_LOSS ? army1 : (result_game.army2 & RESULT_LOSS ? army2 : NULL));
	result_game.killed = army_loss ? army_loss->GetDeadCounts() : 0;
    }
}

void Battle::Arena::ProcessTurnStart(const NetworkEvent &ev) {
    Actions actions;

    bool tower_moved = false;
    bool catapult_moved = false;

    int color = ev.Message->GetInt(HMM2_TURN_COLOR);
    int index = ev.Message->GetInt(HMM2_TURN_UID);
    int new_turn = ev.Message->GetInt(HMM2_TURN_NUMBER);

    if(current_turn != new_turn) {
        /*
         * Start new turn
         */
        DEBUG(DBG_BATTLE, DBG_TRACE, current_turn);

        army1->NewTurn();
        army2->NewTurn();

        current_turn = new_turn;
    }

    /*
     * Find current troop
     */
    Unit* current_troop = army1->GetColor() == color ? army1->FindIndex(index) :
        (army2->GetColor() == color ? army2->FindIndex(index) : NULL);

    if(current_troop == NULL) {
        DEBUG(DBG_BATTLE, DBG_TRACE, "Cannot find troop color=" << color << " index=" << index);
        return;
    }

    DEBUG(DBG_BATTLE, DBG_TRACE, current_troop->String(true));

    /*
     * Focus specified troop and wait for user input
     */
    HumanTurn(*current_troop, actions);

    /*
     * Send actions to the server
     */
    SendActions(actions);
}

void Battle::Arena::PerformActions(const NetworkEvent &ev) {
    std::istringstream Actions(ev.Message->GetBin(HMM2_TURN_ACTIONS));

	// apply actions
    while(1) {
        StreamBuf Stream(1);
        Actions >> Stream;

        if(!Stream.fail()) {
            // apply action
            ApplyAction(Stream);

            // rescan orders
            if(armies_order) Force::UpdateOrderUnits(*army1, *army2, *armies_order);
        }
        else {
            break;
        }
    }

	board.Reset();

    NetworkMessage Msg(HMM2_TURN_COMPLETED);
    Network::Get().QueueOutputMessage(Msg);
}

void Battle::Arena::NetworkTurns(void)
{
    const Settings & conf = Settings::Get();
    LocalEvent & le = LocalEvent::Get();
    bool end_turn = false;

    if(interface && conf.Music() && !Music::isPlaying())
            AGG::PlayMusic(MUS::GetBattleRandom(), false);

    while(!end_turn && le.HandleEvents()) {
        if(Network::Get().IsInputPending()) {
            NetworkEvent ev;
            Network::Get().DequeueInputEvent(ev);
  
            DEBUG(DBG_NETWORK, DBG_INFO, "ProcessNetworkEvents");

            if(ev.OldState != ev.NewState)
            {
#if 0
                if(!ProcessNetworkStateChange(ev))
                    return;
#endif
            }

            if(ev.Message.get() == 0) {
                continue;
            }

            switch(ev.Message->GetType()) {
                case HMM2_TURN_START:
                    ProcessTurnStart(ev);
                    break;
                case HMM2_TURN_ACTION:
                    PerformActions(ev);
                    end_turn = true;
                    break;
            } 
        }

        if(interface)
            interface->NetworkRedraw();
    }

    // end turn: fix result
    if(!army1->isValid() || (result_game.army1 & (RESULT_RETREAT | RESULT_SURRENDER)))
    {
        result_game.army1 |= RESULT_LOSS;
        if(army2->isValid()) result_game.army2 = RESULT_WINS;
    }

    if(!army2->isValid() || (result_game.army2 & (RESULT_RETREAT | RESULT_SURRENDER)))
    {
        result_game.army2 |= RESULT_LOSS;
        if(army1->isValid()) result_game.army1 = RESULT_WINS;
    }
}

void Battle::Arena::RemoteTurn(const Unit & b, Actions & a)
{
    DEBUG(DBG_BATTLE, DBG_WARN, "switch to AI turn");
    AI::BattleTurn(*this, b, a);
}

void Battle::Arena::HumanTurn(const Unit & b, Actions & a)
{
    if(interface)
        interface->HumanTurn(b, a);
}

void Battle::Arena::TowerAction(const Tower & twr)
{
    board.Reset();
    board.SetEnemyQuality(twr);
    const Unit* enemy = GetEnemyMaxQuality(twr.GetColor());

    if(enemy)
    {
	Command cmd(MSG_BATTLE_TOWER);
	cmd.GetStream() << twr.GetType() << enemy->GetUID();

	ApplyAction(cmd.GetStream());
    }
}

void Battle::Arena::CatapultAction(void)
{
    if(catapult)
    {
	ApplyAction(catapult->GetAction(*this).GetStream());
    }
}

Battle::Indexes Battle::Arena::GetPath(const Unit & b, const Position & dst)
{
    Indexes result = board.GetAStarPath(b, dst);

    if(result.size())
    {
	if(IS_DEBUG(DBG_BATTLE, DBG_TRACE))
	{
    	    std::stringstream ss;
	    for(u16 ii = 0; ii < result.size(); ++ii) ss << result[ii] << ", ";
	    DEBUG(DBG_BATTLE, DBG_TRACE, ss.str());
	}
    }

    return result;
}

Battle::Unit* Battle::Arena::GetTroopBoard(s16 index)
{
    return Board::isValidIndex(index) ? board[index].GetUnit() : NULL;
}

const Battle::Unit* Battle::Arena::GetTroopBoard(s16 index) const
{
    return Board::isValidIndex(index) ? board[index].GetUnit() : NULL;
}

const HeroBase* Battle::Arena::GetCommander1(void) const
{
    return army1->GetCommander();
}

const HeroBase* Battle::Arena::GetCommander2(void) const
{
    return army2->GetCommander();
}

u8 Battle::Arena::GetArmyColor1(void) const
{
    return army1->GetColor();
}

u8 Battle::Arena::GetArmyColor2(void) const
{
    return army2->GetColor();
}

u8 Battle::Arena::GetCurrentColor(void) const
{
    return current_color;
}

u8 Battle::Arena::GetOppositeColor(u8 col) const
{
    return col == GetArmyColor1() ? GetArmyColor2() : GetArmyColor1();
}

Battle::Unit* Battle::Arena::GetTroopUID(u32 uid)
{
    Units::iterator it = std::find_if(army1->begin(), army1->end(),
			std::bind2nd(std::mem_fun(&Unit::isUID), uid));

    if(it != army1->end()) return *it;

    it = std::find_if(army2->begin(), army2->end(),
			std::bind2nd(std::mem_fun(&Unit::isUID), uid));

    return it != army2->end() ? *it : NULL;
}

const Battle::Unit* Battle::Arena::GetTroopUID(u32 uid) const
{
    Units::const_iterator it = std::find_if(army1->begin(), army1->end(),
			std::bind2nd(std::mem_fun(&Unit::isUID), uid));

    if(it != army1->end()) return *it;

    it = std::find_if(army2->begin(), army2->end(),
			std::bind2nd(std::mem_fun(&Unit::isUID), uid));

    return it != army2->end() ? *it : NULL;
}

const Battle::Unit* Battle::Arena::GetEnemyMaxQuality(u8 my_color) const
{
    const Unit* res = NULL;
    s32 quality = 0;

    for(Board::const_iterator
	it = board.begin(); it != board.end(); ++it)
    {
	const Unit* enemy = (*it).GetUnit();

	if(enemy && enemy->GetColor() != my_color &&
	    (!enemy->isWide() || enemy->GetTailIndex() != (*it).GetIndex()) &&
	    quality < (*it).GetQuality())
        {
            res = enemy;
            quality = (*it).GetQuality();
        }
    }

    return res;
}

void Battle::Arena::FadeArena(void) const
{
    if(interface) interface->FadeArena();
}

const SpellStorage & Battle::Arena::GetUsageSpells(void) const
{
    return usage_spells;
}

s16 Battle::Arena::GetFreePositionNearHero(u8 color) const
{
    const u8 cells1[] = { 11, 22, 33 };
    const u8 cells2[] = { 21, 32, 43 };
    const u8* cells = NULL;

    if(army1->GetColor() == color) cells = cells1;
    else
    if(army2->GetColor() == color) cells = cells2;

    if(cells) for(u8 ii = 0; ii < 3; ++ii)
	if(board[cells[ii]].isPassable1(true) && NULL == board[cells[ii]].GetUnit()) return cells[ii];

    return -1;
}

bool Battle::Arena::CanSurrenderOpponent(u8 color) const
{
    const HeroBase* hero1 = GetCommander(color, false); // enemy
    const HeroBase* hero2 = GetCommander(color, true);
    return hero1 && hero1->GetType() == Skill::Primary::HEROES && hero2 &&
	world.GetKingdom(hero2->GetColor()).GetCastles().size();
}

bool Battle::Arena::CanRetreatOpponent(u8 color) const
{
    const HeroBase* hero = army1->GetColor() == color ? army1->GetCommander() : army2->GetCommander();
    return hero && hero->GetType() == Skill::Primary::HEROES && NULL == hero->inCastle() &&
	world.GetKingdom(hero->GetColor()).GetCastles().size();
}

bool Battle::Arena::isDisableCastSpell(const Spell & spell, std::string* msg)
{
    const HeroBase* hero1 = army1->GetCommander();
    const HeroBase* hero2 = army2->GetCommander();
    const HeroBase* current_commander = GetCurrentCommander();

    // check sphere negation (only for heroes)
    if((hero1 && hero1->HasArtifact(Artifact::SPHERE_NEGATION)) ||
       (hero2 && hero2->HasArtifact(Artifact::SPHERE_NEGATION)))
    {
        if(msg) *msg = _("The Sphere of Negation artifact is in effect for this battle, disabling all combat spells.");
        return true;
    }

    // check casted
    if(current_commander)
    {
	if(current_commander->Modes(Heroes::SPELLCASTED))
	{
	    if(msg) *msg = _("You have already cast a spell this round.");
	    return true;
	}

	if(spell == Spell::EARTHQUAKE && !castle)
	{
	    *msg = _("That spell will affect no one!");
	    return true;
	}
	else
	if(spell.isSummon())
	{
	    const Unit* elem = GetCurrentForce().FindMode(CAP_SUMMONELEM);
	    bool affect = true;

	    if(elem) switch(spell())
	    {
		case Spell::SUMMONEELEMENT: if(elem->GetID() != Monster::EARTH_ELEMENT) affect = false; break;
		case Spell::SUMMONAELEMENT: if(elem->GetID() != Monster::AIR_ELEMENT) affect = false; break;
		case Spell::SUMMONFELEMENT: if(elem->GetID() != Monster::FIRE_ELEMENT) affect = false; break;
		case Spell::SUMMONWELEMENT: if(elem->GetID() != Monster::WATER_ELEMENT) affect = false; break;
		default: break;
	    }
	    if(!affect)
	    {
		*msg = _("You may only summon one type of elemental per combat.");
		return true;
	    }

	    if(0 > GetFreePositionNearHero(current_color))
	    {
		*msg = _("There is no open space adjacent to your hero to summon an Elemental to.");
		return true;
	    }
	}
	else
	if(spell.isValid())
	{
	    // check army
	    for(Board::const_iterator
		it = board.begin(); it != board.end(); ++it)
	    {
		const Battle::Unit* b = (*it).GetUnit();

		if(b)
		{
		    if(b->AllowApplySpell(spell, current_commander, NULL))
			return false;
		}
		else
		// check graveyard
		if(GraveyardAllowResurrect((*it).GetIndex(), spell))
			return false;
	    }
	    *msg = _("That spell will affect no one!");
	    return true;
	}
    }

    // may be check other..
    /*
    */

    return false;
}

bool Battle::Arena::GraveyardAllowResurrect(s16 index, const Spell & spell) const
{
    const HeroBase* hero = GetCurrentCommander();
    const Unit* killed = GetTroopUID(graveyard.GetLastTroopUID(index));
    const Unit* tail = killed && killed->isWide() ?
	    GetTroopUID(graveyard.GetLastTroopUID(killed->GetTailIndex())) : NULL;

    return killed && (! killed->isWide() || killed == tail) &&
	hero && spell.isResurrect() && killed->AllowApplySpell(spell, hero, NULL);
}

const Battle::Unit* Battle::Arena::GraveyardLastTroop(s16 index) const
{
    return GetTroopUID(graveyard.GetLastTroopUID(index));
}

Battle::Indexes Battle::Arena::GraveyardClosedCells(void) const
{
    return graveyard.GetClosedCells();
}

void Battle::Arena::SetCastleTargetValue(u8 target, u8 value)
{
    switch(target)
    {
        case CAT_WALL1: board[8].SetObject(value); break;
        case CAT_WALL2: board[29].SetObject(value); break;
        case CAT_WALL3: board[73].SetObject(value); break;
        case CAT_WALL4: board[96].SetObject(value); break;

        case CAT_TOWER1:if(towers[0] && towers[0]->isValid()) towers[0]->SetDestroy(); break;
        case CAT_TOWER2:if(towers[2] && towers[2]->isValid()) towers[2]->SetDestroy(); break;
        case CAT_TOWER3:if(towers[1] && towers[1]->isValid()) towers[1]->SetDestroy(); break;

        case CAT_BRIDGE:
	    if(bridge->isValid())
	    {
                if(interface) interface->RedrawBridgeAnimation(true);
		bridge->SetDown(true);
		bridge->SetDestroy();
	    }
	    break;

        default: break;
    }
}

u8 Battle::Arena::GetCastleTargetValue(u8 target) const
{
    switch(target)
    {
        case CAT_WALL1: return board[8].GetObject();
        case CAT_WALL2: return board[29].GetObject();
        case CAT_WALL3: return board[73].GetObject();
        case CAT_WALL4: return board[96].GetObject();

        case CAT_TOWER1:return towers[0] && towers[0]->isValid();
        case CAT_TOWER2:return towers[2] && towers[2]->isValid();
        case CAT_TOWER3:return towers[1] && towers[1]->isValid();

        case CAT_BRIDGE:return bridge->isValid();

        default: break;
    }
    return 0;
}

std::vector<u8> Battle::Arena::GetCastleTargets(void) const
{
    std::vector<u8> targets;
    targets.reserve(8);

    // check walls
    if(0 != board[8].GetObject())  targets.push_back(CAT_WALL1);
    if(0 != board[29].GetObject()) targets.push_back(CAT_WALL2);
    if(0 != board[73].GetObject()) targets.push_back(CAT_WALL3);
    if(0 != board[96].GetObject()) targets.push_back(CAT_WALL4);

    // check right/left towers
    if(towers[0] && towers[0]->isValid()) targets.push_back(CAT_TOWER1);
    if(towers[2] && towers[2]->isValid()) targets.push_back(CAT_TOWER2);

    return targets;
}

StreamBase & Battle::operator<< (StreamBase & msg, const Arena & a)
{
    msg <<
	a.current_turn << a.board <<
	*a.army1 << *a.army2;

    const HeroBase* hero1 = a.army1->GetCommander();
    const HeroBase* hero2 = a.army2->GetCommander();

    if(hero1)
	msg << hero1->GetType() << *hero1;
    else
	msg << static_cast<u8>(Skill::Primary::UNDEFINED);

    if(hero2)
	msg << hero1->GetType() << *hero2;
    else
	msg << static_cast<u8>(Skill::Primary::UNDEFINED);

    return msg;
}

StreamBase & Battle::operator>> (StreamBase & msg, Arena & a)
{
    msg >> a.current_turn >> a.board >>
	*a.army1 >> *a.army2;

    u8 type;
    HeroBase* hero1 = a.army1->GetCommander();
    HeroBase* hero2 = a.army2->GetCommander();

    msg >> type;
    if(hero1 && type == hero1->GetType()) msg >> *hero1;

    msg >> type;
    if(hero2 && type == hero2->GetType()) msg >> *hero2;

    return msg;
}

const HeroBase* Battle::Arena::GetCommander(u8 color, bool invert) const
{
    const HeroBase* commander = NULL;

    if(army1->GetColor() == color)
    {
	commander = invert ? army2->GetCommander() : army1->GetCommander();
    }
    else
    {
	commander = invert ? army1->GetCommander() : army2->GetCommander();
    }

    return commander;
}

const HeroBase* Battle::Arena::GetCurrentCommander(void) const
{
    return GetCommander(current_color);
}

Battle::Unit* Battle::Arena::CreateElemental(const Spell & spell)
{
    const HeroBase* hero = GetCurrentCommander();
    const s16 pos = GetFreePositionNearHero(current_color);

    if(0 > pos || !hero)
    {
	DEBUG(DBG_BATTLE, DBG_WARN, "internal error");
	return NULL;
    }

    Force & army = GetCurrentForce();
    Unit* elem = army.FindMode(CAP_SUMMONELEM);
    bool affect = true;

    if(elem) switch(spell())
    {
        case Spell::SUMMONEELEMENT: if(elem->GetID() != Monster::EARTH_ELEMENT) affect = false; break;
        case Spell::SUMMONAELEMENT: if(elem->GetID() != Monster::AIR_ELEMENT) affect = false; break;
        case Spell::SUMMONFELEMENT: if(elem->GetID() != Monster::FIRE_ELEMENT) affect = false; break;
        case Spell::SUMMONWELEMENT: if(elem->GetID() != Monster::WATER_ELEMENT) affect = false; break;
        default: break;
    }

    if(!affect)
    {
	DEBUG(DBG_BATTLE, DBG_WARN, "other elemental summon");
	return NULL;
    }

    Monster mons(spell);

    if(! mons.isValid())
    {
	DEBUG(DBG_BATTLE, DBG_WARN, "unknown id");
	return NULL;
    }

    DEBUG(DBG_BATTLE, DBG_TRACE, mons.GetName() << ", position: " << pos);
    u16 count = spell.ExtraValue() * hero->GetPower();
    u8 acount = hero->HasArtifact(Artifact::BOOK_ELEMENTS);
    if(acount) count *= acount * 2;

    elem = new Unit(Troop(mons, count), pos, hero == army2->GetCommander());

    if(elem)
    {
        elem->SetModes(CAP_SUMMONELEM);
	elem->SetArmy(hero->GetArmy());
	if(interface) elem->InitContours();
	army.push_back(elem);
    }
    else
    {
	DEBUG(DBG_BATTLE, DBG_WARN, "is NULL");
    }

    return elem;
}

Battle::Unit* Battle::Arena::CreateMirrorImage(Unit & b, s16 pos)
{
    Unit* image = new Unit(b, pos, b.isReflect());

    if(image)
    {
	b.SetMirror(image);
	image->SetArmy(*b.GetArmy());
	image->SetMirror(&b);
	image->SetModes(CAP_MIRRORIMAGE);
	if(interface) image->InitContours();
	b.SetModes(CAP_MIRROROWNER);

	GetCurrentForce().push_back(image);
    }
    else
    {
	DEBUG(DBG_BATTLE, DBG_WARN, "internal error");
    }

    return image;
}

u8 Battle::Arena::GetObstaclesPenalty(const Unit & attacker, const Unit & defender) const
{
    if(defender.Modes(CAP_TOWER) || attacker.Modes(CAP_TOWER)) return 0;

    // check golden bow artifact
    const HeroBase* enemy = attacker.GetCommander();
    if(enemy && enemy->HasArtifact(Artifact::GOLDEN_BOW)) return 0;

    u8 result = 0;
    const u8 step = Settings::Get().QVGA() ? CELLW2 / 3 : CELLW / 3;

    if(castle)
    {
	// archery skill
	if(enemy && Skill::Level::NONE != enemy->GetLevelSkill(Skill::Secondary::ARCHERY)) return 0;

	// attacker is castle owner
	if(attacker.GetColor() == castle->GetColor() &&
	    !attacker.OutOfWalls()) return 0;

	if(defender.GetColor() == castle->GetColor() &&
	    defender.OutOfWalls()) return 0;

	// check castle walls defensed
	const Points points = GetLinePoints(attacker.GetBackPoint(), defender.GetBackPoint(), step);

	for(Points::const_iterator
	    it = points.begin(); it != points.end(); ++it)
	{
	    if(0 == board[8].GetObject() && (board[8].GetPos() & *it)) return 0;
	    else
	    if(0 == board[29].GetObject() && (board[29].GetPos() & *it)) return 0;
	    else
	    if(0 == board[73].GetObject() && (board[73].GetPos() & *it)) return 0;
	    else
	    if(0 == board[96].GetObject() && (board[96].GetPos() & *it)) return 0;
	}

	result = 1;
    }
    else
    if(Settings::Get().ExtBattleObjectsArchersPenalty())
    {
	const Points points = GetLinePoints(attacker.GetBackPoint(), defender.GetBackPoint(), step);
	Indexes indexes;
	indexes.reserve(points.size());

	for(Points::const_iterator
	    it = points.begin(); it != points.end(); ++it)
	{
	    const s16 index = board.GetIndexAbsPosition(*it);
	    if(Board::isValidIndex(index)) indexes.push_back(index);
	}

	if(indexes.size())
	{
	    std::sort(indexes.begin(), indexes.end());
	    indexes.resize(std::distance(indexes.begin(), std::unique(indexes.begin(), indexes.end())));
	}

	for(Indexes::const_iterator
	    it = indexes.begin(); it != indexes.end(); ++it)
	{
	    // obstacles
	    switch(board[*it].GetObject())
	    {
		    // tree
		    case 0x82:
		    // trock
		    case 0x85:
		    // tree
		    case 0x89:
		    // tree
		    case 0x8D:
		    // rock
		    case 0x95:
		    case 0x96:
		    // stub
		    case 0x9A:
		    // dead tree
		    case 0x9B:
		    // tree
		    case 0x9C: ++result; break;

		    default: break;
	    }
	}

	if(enemy)
	{
	    switch(enemy->GetLevelSkill(Skill::Secondary::ARCHERY))
	    {
		case Skill::Level::BASIC:	if(result < 2) return 0; break;
		case Skill::Level::ADVANCED:	if(result < 3) return 0; break;
		case Skill::Level::EXPERT:	return 0;
		default: break;
	    }
	}
    }

    return result;
}

Battle::Force & Battle::Arena::GetForce1(void)
{
    return *army1;
}

Battle::Force & Battle::Arena::GetForce2(void)
{
    return *army2;
}

Battle::Force & Battle::Arena::GetForce(u8 color, bool invert)
{
    if(army1->GetColor() == color)
        return invert ? *army2 : *army1;

    return invert ? *army1 : *army2;
}

Battle::Force & Battle::Arena::GetCurrentForce(void)
{
    return GetForce(current_color, false);
}

ICN::icn_t Battle::Arena::GetICNCovr(void) const
{
    return icn_covr;
}

u16 Battle::Arena::GetCurrentTurn(void) const
{
    return current_turn;
}

Battle::Result & Battle::Arena::GetResult(void)
{
    return result_game;
}

bool Battle::Arena::CanBreakAutoBattle(void) const
{
    return (auto_battle & current_color) && GetCurrentCommander() &&
	CONTROL_AI != GetCurrentCommander()->GetControl();
}

void Battle::Arena::BreakAutoBattle(void)
{
    auto_battle &= ~current_color;
}

void Battle::Arena::SendActions(Actions &actions) {
    std::ostringstream actions_data;

    for(Actions::iterator i = actions.begin() ; i != actions.end() ; i++) {
        actions_data << i->GetStream();
    }

    NetworkMessage Msg(HMM2_TURN_SUBMIT);
    Msg.add_bin_chunk(HMM2_TURN_ACTIONS, actions_data.str());
    Network::Get().QueueOutputMessage(Msg);
}
