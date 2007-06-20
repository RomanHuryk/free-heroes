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

#include "agg.h"
#include "castle.h"

void Castle::RedrawBuilding(const Point & dst_pt)
{
    switch(race)
    {
	case Race::KNGT:
	{
	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNKTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if( BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if((DWELLING_MONSTER2 | DWELLING_UPGRADE2) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE2 & building ? "TWNKUP_1.ICN" : "TWNKDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_TAVERN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKTVRN.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite = AGG::GetICN("TWNKMAGE.ICN", level - 1);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER5 | DWELLING_UPGRADE5) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE5 & building ? "TWNKUP_4.ICN" : "TWNKDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER6 | DWELLING_UPGRADE6) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE6 & building ? "TWNKUP_5.ICN" : "TWNKDW_5.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER3 | DWELLING_UPGRADE3) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE3 & building ? "TWNKUP_2.ICN" : "TWNKDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 & building ? "TWNKUP_3.ICN" : "TWNKDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNKSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNKBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNKDOCK.ICN" : "TWNKEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	case Race::BARB:
	{
	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if( BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER6 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBDW_5.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite1 = AGG::GetICN("TWNBMAGE.ICN", level - 1);
		display.Blit(sprite1, dst_pt.x + sprite1.x(), dst_pt.y + sprite1.y());

		const Sprite & sprite2 = AGG::GetICN("TWNBEXT2.ICN", 0);
		display.Blit(sprite2, dst_pt.x + sprite2.x(), dst_pt.y + sprite2.y());
	    }

	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNBTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER3 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_TAVERN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBTVRN.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER2 | DWELLING_UPGRADE2) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE2 & building ? "TWNBUP_1.ICN" : "TWNBDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 & building ? "TWNBUP_3.ICN" : "TWNBDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER5 | DWELLING_UPGRADE5) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE5 & building ? "TWNBUP_4.ICN" : "TWNBDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNBSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNBBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNBDOCK.ICN" : "TWNBEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	case Race::SORC:
	{
	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER6 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSDW_5.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite = AGG::GetICN("TWNSMAGE.ICN", level - 1);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    
	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNSTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER3 | DWELLING_UPGRADE3) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE3 & building ? "TWNSUP_2.ICN" : "TWNSDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNSBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNSDOCK.ICN" : "TWNSEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER2 | DWELLING_UPGRADE2) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE2 & building ? "TWNSUP_1.ICN" : "TWNSDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_TAVERN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSTVRN.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((BUILD_WEL2 & building) && (BUILD_STATUE & building))
	    {
		const Sprite & sprite = AGG::GetICN("TWNSEXT1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    if(BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 & building ? "TWNSUP_3.ICN" : "TWNSDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER5 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNSDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	case Race::WRLK:
	{
	    if(DWELLING_MONSTER5 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER3 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNWTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNWBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNWDOCK.ICN" : "TWNWEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite = AGG::GetICN("TWNWMAGE.ICN", level - 1);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_TAVERN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWTVRN.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 & building ? "TWNWUP_3.ICN" : "TWNWDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_UPGRADE7 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWUP5B.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    if((DWELLING_MONSTER6 | DWELLING_UPGRADE6) & building)
	    {
	        const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE6 & building ? "TWNWUP_5.ICN" : "TWNWDW_5.ICN", 0);
	        display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNWWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	case Race::WZRD:
	{
	    if((DWELLING_MONSTER6 | DWELLING_UPGRADE6) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE6 & building ? "TWNZUP_5.ICN" : "TWNZDW_5.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNZTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_TAVERN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZTVRN.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNZBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNZDOCK.ICN" : "TWNZEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER3 | DWELLING_UPGRADE3) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE3 ? "TWNZUP_2.ICN" : "TWNZDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER5 | DWELLING_UPGRADE5) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE5 ? "TWNZUP_4.ICN" : "TWNZDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite = AGG::GetICN("TWNZMAGE.ICN", level - 1);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 ? "TWNZUP_3.ICN" : "TWNZDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNZWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	case Race::NECR:
	{
	    if(BUILD_SPEC & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNSPEC.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CASTLE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNCSTL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	    else
	    {
		const Sprite & sprite = AGG::GetICN("TWNNTENT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_CAPTAIN & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNCAPT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_LEFTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNLTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_RIGHTTURRET & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNRTUR.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER6 | DWELLING_UPGRADE6) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE6 ? "TWNNUP_5.ICN" : "TWNNDW_5.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MOAT & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNMOAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(HaveNearlySea())
	    {
		const Sprite & sprite = AGG::GetICN(BUILD_BOAT & building ? "TWNNBOAT.ICN" : (BUILD_SHIPYARD & building ? "TWNNDOCK.ICN" : "TWNNEXT0.ICN"), 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_THIEVESGUILD & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNTHIE.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER3 | DWELLING_UPGRADE3) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE3 ? "TWNNUP_2.ICN" : "TWNNDW_2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER5 | DWELLING_UPGRADE5) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE5 ? "TWNNUP_4.ICN" : "TWNNDW_4.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER2 | DWELLING_UPGRADE2) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE2 ? "TWNNUP_1.ICN" : "TWNNDW_1.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if((DWELLING_MONSTER4 | DWELLING_UPGRADE4) & building)
	    {
		const Sprite & sprite = AGG::GetICN(DWELLING_UPGRADE4 ? "TWNNUP_3.ICN" : "TWNNDW_3.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(DWELLING_MONSTER1 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNDW_0.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(u8 level = GetLevelMageGuild())
	    {
		const Sprite & sprite = AGG::GetICN("TWNNMAGE.ICN", (level - 1) * 6);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WEL2 & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNWEL2.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_MARKETPLACE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNMARK.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_STATUE & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNSTAT.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }

	    if(BUILD_WELL & building)
	    {
		const Sprite & sprite = AGG::GetICN("TWNNWELL.ICN", 0);
		display.Blit(sprite, dst_pt.x + sprite.x(), dst_pt.y + sprite.y());
	    }
	}
	    break;
	
	default: return;
    }
}

Rect Castle::GetCoordBuilding(building_t building, const Point & pt)
{
    switch(building)
    {
	case BUILD_THIEVESGUILD:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 0, pt.y + 130, 50, 60);
		case Race::BARB:	return Rect(pt.x + 478, pt.y + 100, 76, 42);
		case Race::SORC:	return Rect(pt.x + 423, pt.y + 165, 65, 49);
		case Race::WRLK:	return Rect(pt.x + 479, pt.y + 100, 39, 52);
		case Race::WZRD:	return Rect(pt.x + 507, pt.y + 55, 47, 42);
		case Race::NECR:	return Rect(pt.x + 291, pt.y + 134, 43, 59);
		default: break;
	    }
	    break;

        case BUILD_TAVERN:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 350, pt.y + 110, 46, 56);
		case Race::BARB:	return Rect(pt.x + 0, pt.y + 205, 125, 60);
		case Race::SORC:	return Rect(pt.x + 494, pt.y + 140, 131, 87);
		case Race::WRLK:	return Rect(pt.x + 525, pt.y + 109, 60, 48);
		case Race::WZRD:	return Rect(pt.x, pt.y + 160, 118, 50);
		default: break;
	    }
	    break;

        case BUILD_SHIPYARD:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 537, pt.y + 221, 103, 33);
		case Race::BARB:	return Rect(pt.x + 535, pt.y + 210, 105, 45);
		case Race::SORC:	return Rect(pt.x + 0, pt.y + 220, 134, 35);
		case Race::WRLK:	return Rect(pt.x + 520, pt.y + 206, 120, 47);
		case Race::WZRD:	return Rect(pt.x, pt.y + 218, 185, 35);
		case Race::NECR:	return Rect(pt.x + 516, pt.y + 221, 124, 28);
		default: break;
	    }
	    break;

        case BUILD_WELL:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 194, pt.y + 225, 29, 27);
		case Race::BARB:	return Rect(pt.x + 272, pt.y + 215, 44, 32);
		case Race::SORC:	return Rect(pt.x + 346, pt.y + 209, 43, 25);
		case Race::WRLK:	return Rect(pt.x + 348, pt.y + 221, 63, 30);
		case Race::WZRD:	return Rect(pt.x + 254, pt.y + 143, 19, 28);
		case Race::NECR:	return Rect(pt.x + 217, pt.y + 225, 23, 26);
		default: break;
	    }
	    break;

        case BUILD_STATUE:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 480, pt.y + 205, 45, 50);
		case Race::BARB:	return Rect(pt.x + 467, pt.y + 102, 31, 49);
		case Race::SORC:	return Rect(pt.x + 158, pt.y + 173, 17, 58);
		case Race::WRLK:	return Rect(pt.x + 473, pt.y + 172, 45, 51);
		case Race::WZRD:	return Rect(pt.x + 464, pt.y + 58, 26, 62);
		case Race::NECR:	return Rect(pt.x + 374, pt.y + 174, 26, 70);
		default: break;
	    }
	    break;

        case BUILD_MARKETPLACE:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 220, pt.y + 144, 115, 20);
		case Race::BARB:	return Rect(pt.x + 224, pt.y + 168, 52, 36);
		case Race::SORC:	return Rect(pt.x + 412, pt.y + 122, 56, 40);
		case Race::WRLK:	return Rect(pt.x + 391, pt.y + 185, 70, 26);
		case Race::WZRD:	return Rect(pt.x + 254, pt.y + 176, 105, 39);
		case Race::NECR:	return Rect(pt.x + 415, pt.y + 216, 85, 35);
		default: break;
	    }
	    break;

        case BUILD_WEL2:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 288, pt.y + 97, 63, 18);
		case Race::BARB:	return Rect(pt.x + 252, pt.y + 120, 44, 16);
		case Race::SORC:	return Rect(pt.x + 135, pt.y + 200, 63, 31);
		case Race::WRLK:	return Rect(pt.x + 69, pt.y + 46, 24, 124);
		case Race::WZRD:	return Rect(pt.x + 234, pt.y + 225, 107, 22);
		case Race::NECR:	return Rect(pt.x + 275, pt.y + 206, 68, 39);
		default: break;
	    }
	    break;

        case BUILD_MOAT:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 53, pt.y + 150, 93, 30);
		case Race::BARB:	return Rect(pt.x + 113, pt.y + 155, 106, 30);
		case Race::SORC:	return Rect(pt.x + 143, pt.y + 169, 98, 11);
		case Race::WRLK:	return Rect(pt.x + 327, pt.y + 166, 66, 17);
		case Race::WZRD:	return Rect(pt.x, pt.y + 91, 198, 11);
		case Race::NECR:	return Rect(pt.x + 336, pt.y + 169, 71, 15);
		default: break;
	    }
	    break;

        case BUILD_SPEC:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 0, pt.y + 80, 250, 20);
		case Race::BARB:	return Rect(pt.x + 223, pt.y + 79, 124, 27);
		case Race::SORC:	return Rect(pt.x + 147, pt.y + 0, 252, 30);
		case Race::WRLK:	return Rect(pt.x, pt.y + 162, 70, 77);
		case Race::WZRD:	return Rect(pt.x + 304, pt.y + 111, 95, 50);
		case Race::NECR:	return Rect(pt.x, pt.y, 640, 54);
		default: break;
	    }
	    break;

        case BUILD_CASTLE:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 0, pt.y + 55, 290, 85);
		case Race::BARB:	return Rect(pt.x + 0, pt.y + 88, 202, 62);
		case Race::SORC:	return Rect(pt.x + 0, pt.y + 67, 198, 100);
		case Race::WRLK:	return Rect(pt.x + 268, pt.y + 35, 131, 129);
		case Race::WZRD:	return Rect(pt.x, pt.y + 48, 187, 39);
		case Race::NECR:	return Rect(pt.x + 322, pt.y + 63, 73, 97);
		default: break;
	    }
	    break;

        case BUILD_CAPTAIN:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 293, pt.y + 109, 48, 27);
		case Race::BARB:	return Rect(pt.x + 210, pt.y + 104, 40, 35);
		case Race::SORC:	return Rect(pt.x + 238, pt.y + 136, 32, 34);
		case Race::WRLK:	return Rect(pt.x + 420, pt.y + 102, 52, 60);
		case Race::WZRD:	return Rect(pt.x + 210, pt.y + 55, 28, 32);
		case Race::NECR:	return Rect(pt.x + 423, pt.y + 126, 41, 46);
		default: break;
	    }
	    break;

        case BUILD_MAGEGUILD1:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 398, pt.y + 150, 58, 30);
		case Race::BARB:	return Rect(pt.x + 348, pt.y + 118, 50, 25);
		case Race::SORC:	return Rect(pt.x + 285, pt.y + 32, 55, 129);
		case Race::WRLK:	return Rect(pt.x + 590, pt.y + 135, 50, 35);
		case Race::WZRD:	return Rect(pt.x + 583, pt.y + 93, 57, 28);
		case Race::NECR:	return Rect(pt.x + 565, pt.y + 131, 73, 74);
		default: break;
	    }
	    break;

	case BUILD_MAGEGUILD2:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 398, pt.y + 128, 58, 52);
		case Race::BARB:	return Rect(pt.x + 348, pt.y + 94, 50, 49);
		case Race::SORC:	return Rect(pt.x + 285, pt.y + 32, 55, 129);
		case Race::WRLK:	return Rect(pt.x + 590, pt.y + 108, 50, 60);
		case Race::WZRD:	return Rect(pt.x + 585, pt.y + 69, 55, 50);
		case Race::NECR:	return Rect(pt.x + 568, pt.y + 102, 62, 104);
		default: break;
	    }
	    break;

	case BUILD_MAGEGUILD3:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 398, pt.y + 105, 58, 75);
		case Race::BARB:	return Rect(pt.x + 348, pt.y + 72, 50, 72);
		case Race::SORC:	return Rect(pt.x + 285, pt.y + 32, 55, 129);
		case Race::WRLK:	return Rect(pt.x + 590, pt.y + 77, 50, 90);
		case Race::WZRD:	return Rect(pt.x + 585, pt.y + 44, 55, 78);
		case Race::NECR:	return Rect(pt.x + 570, pt.y + 79, 56, 130);
		default: break;
	    }
	    break;

	case BUILD_MAGEGUILD4:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 398, pt.y + 85, 58, 95);
		case Race::BARB:	return Rect(pt.x + 348, pt.y + 48, 50, 96);
		case Race::SORC:	return Rect(pt.x + 285, pt.y + 32, 55, 129);
		case Race::WRLK:	return Rect(pt.x + 590, pt.y + 45, 50, 125);
		case Race::WZRD:	return Rect(pt.x + 585, pt.y + 20, 54, 102);
		case Race::NECR:	return Rect(pt.x + 570, pt.y + 61, 60, 146);
		default: break;
	    }
	    break;

	case BUILD_MAGEGUILD5:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 398, pt.y + 55, 58, 125);
		case Race::BARB:	return Rect(pt.x + 348, pt.y + 20, 50, 124);
		case Race::SORC:	return Rect(pt.x + 285, pt.y + 32, 55, 129);
		case Race::WRLK:	return Rect(pt.x + 590, pt.y + 14, 50, 155);
		case Race::WZRD:	return Rect(pt.x + 585 , pt.y, 57, 122);
		case Race::NECR:	return Rect(pt.x + 570, pt.y + 45, 61, 162);
		default: break;
	    }
	    break;

	case BUILD_TENT:
	    switch(race)
	    {
                case Race::KNGT:        return Rect(pt.x + 82, pt.y + 132, 42, 30);
                case Race::BARB:        return Rect(pt.x + 53, pt.y + 119, 67, 35);
                case Race::SORC:        return Rect(pt.x + 88, pt.y + 145, 62, 36);
                case Race::WRLK:        return Rect(pt.x + 308, pt.y + 140, 52, 28);
                case Race::WZRD:        return Rect(pt.x + 60, pt.y + 68, 46, 33);
                case Race::NECR:        return Rect(pt.x + 333, pt.y + 131, 49, 51);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER1:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 195, pt.y + 175, 50, 40);
		case Race::BARB:	return Rect(pt.x + 258, pt.y + 142, 71, 41);
		case Race::SORC:	return Rect(pt.x + 478, pt.y + 70, 92, 62);
		case Race::WRLK:	return Rect(pt.x, pt.y + 63, 68, 53);
		case Race::WZRD:	return Rect(pt.x + 459, pt.y + 181, 45, 32);
		case Race::NECR:	return Rect(pt.x + 404, pt.y + 181, 56, 25);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER2:
	case DWELLING_UPGRADE2:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 145, pt.y + 155, 58, 20);
		case Race::BARB:	return Rect(pt.x + 152, pt.y + 190, 68, 50);
		case Race::SORC:	return Rect(pt.x + 345, pt.y + 149, 70, 56);
		case Race::WRLK:	return Rect(pt.x + 248, pt.y + 192, 60, 55);
		case Race::WZRD:	return Rect(pt.x + 253, pt.y + 69, 90, 29);
		case Race::NECR:	return Rect(pt.x + 147, pt.y + 184, 92, 32);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER3:
	case DWELLING_UPGRADE3:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 250, pt.y + 177, 70, 50);
		case Race::BARB:	return Rect(pt.x + 582, pt.y + 81, 58, 40);
		case Race::SORC:	return Rect(pt.x + 90, pt.y + 180, 56, 21);
		case Race::WRLK:	return Rect(pt.x + 504, pt.y + 53, 38, 30);
		case Race::WZRD:	return Rect(pt.x + 156, pt.y + 139, 74, 51);
		case Race::NECR:	return Rect(pt.x + 108, pt.y + 69, 117, 91);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER4:
	case DWELLING_UPGRADE4:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 328, pt.y + 195, 100, 50);
		case Race::BARB:	return Rect(pt.x + 509, pt.y + 148, 123, 57);
		case Race::SORC:	return Rect(pt.x + 208, pt.y + 182, 127, 55);
		case Race::WRLK:	return Rect(pt.x + 154, pt.y + 168, 171, 36);
		case Race::WZRD:	return Rect(pt.x + 593, pt.y + 187, 47, 28);
		case Race::NECR:	return Rect(pt.x, pt.y + 154, 140, 74);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER5:
 	case DWELLING_UPGRADE5:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 0, pt.y + 200, 150, 55);
		case Race::BARB:	return Rect(pt.x + 331, pt.y + 186, 121, 44);
		case Race::SORC:	return Rect(pt.x + 345, pt.y + 231, 165, 21);
		case Race::WRLK:	return Rect(pt.x + 149, pt.y + 98, 95, 58);
		case Race::WZRD:	return Rect(pt.x + 417, pt.y + 25, 35, 140);
		case Race::NECR:	return Rect(pt.x + 235, pt.y + 136, 53, 70);
		default: break;
	    }
	    break;

	case DWELLING_MONSTER6:
 	case DWELLING_UPGRADE6:
 	case DWELLING_UPGRADE7:
	    switch(race)
	    {
		case Race::KNGT:	return Rect(pt.x + 465, pt.y + 85, 175, 110);
		case Race::BARB:	return Rect(pt.x + 407, pt.y + 13, 109, 80);
		case Race::SORC:	return Rect(pt.x + 202, pt.y + 38, 42, 65);
		case Race::WRLK:	return Rect(pt.x + 98, pt.y + 25, 55, 229);
		case Race::WZRD:	return Rect(pt.x + 196, pt.y + 7, 129, 38);
		case Race::NECR:	return Rect(pt.x + 468, pt.y + 112, 92, 78);
		default: break;
	    }
	    break;

	default: break;
    }

    return Rect();
}
