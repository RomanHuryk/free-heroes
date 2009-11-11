/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov                               *
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

#include <cstdlib>
#include <algorithm>
#include <functional>

#include "sdlnet.h"
#include "settings.h"
#include "server.h"
#include "error.h"
#include "remoteclient.h"
#include "localclient.h"
#include "network.h"

#ifdef WITH_NET

const char* Network::GetMsgString(u16 msg)
{
    switch(msg)
    {
        case MSG_RAW:           return "MSG_RAW";

        case MSG_PING:          return "MSG_PING";
        case MSG_READY:         return "MSG_READY";
        case MSG_MESSAGE:       return "MSG_MESSAGE";

        case MSG_HELLO:         return "MSG_HELLO";
        case MSG_LOGOUT:        return "MSG_LOGOUT";
        case MSG_SHUTDOWN:      return "MSG_SHUTDOWN";

        case MSG_MAPS_INFO:     return "MSG_MAPS_INFO";
        case MSG_MAPS_INFO_GET: return "MSG_MAPS_INFO_GET";
        case MSG_MAPS_INFO_SET:	return "MSG_MAPS_INFO_SET";

        case MSG_MAPS_LOAD:	return "MSG_MAPS_LOAD";
        case MSG_MAPS_LOAD_ERR:	return "MSG_MAPS_LOAD_ERR";

        case MSG_MAPS_LIST:     return "MSG_MAPS_LIST";
        case MSG_MAPS_LIST_GET: return "MSG_MAPS_LIST_GET";

        case MSG_PLAYERS:       return "MSG_PLAYERS";
        case MSG_PLAYERS_GET:   return "MSG_PLAYERS_GET";

        case MSG_YOUR_TURN:     return "MSG_YOUR_TURN";
        case MSG_END_TURN:      return "MSG_END_TURN";
        case MSG_HEROES_MOVE:   return "MSG_HEROES_MOVE";
        case MSG_HEROES:        return "MSG_HEROES";
        case MSG_CASTLE:        return "MSG_CASTLE";
        case MSG_SPELL:         return "MSG_SPELL";
        case MSG_MAPS:          return "MSG_MAPS";
        case MSG_KINGDOM:       return "MSG_KINGDOM";
        case MSG_WORLD:         return "MSG_WORLD";

        default: break;
    }

    return "MSG_UNKNOWN";
}

msg_t Network::GetMsg(u16 msg)
{
    return msg < MSG_UNKNOWN ? static_cast<msg_t>(msg) : MSG_UNKNOWN;
}

bool Network::MsgIsBroadcast(u16 msg)
{
    switch(msg)
    {
	case MSG_SHUTDOWN:
	case MSG_MESSAGE:
	    return true;
    }
    return false;
}

int Network::RunDedicatedServer(void)
{
    Settings & conf = Settings::Get();

    Network::SetProtocolVersion(static_cast<u16>((conf.MajorVersion() << 8)) | conf.MinorVersion());
    
    if(SDL::Init(INIT_TIMER))
    try
    {
        std::atexit(SDL::Quit);

        FH2Server & server = FH2Server::Get();

        if(! server.Bind(conf.GetPort()))
        {
            DEBUG(DBG_NETWORK , DBG_WARN, "Network::RunDedicatedServer: " << Network::GetError());
            return -1;
        }

	conf.SetModes(Settings::DEDICATEDSERVER);
	conf.SetGameType(Game::NETWORK);

        return FH2Server::callbackCreateThread(&server);
    }
    catch(std::bad_alloc)
    {
    }
    catch(Error::Exception)
    {
        conf.Dump();
    }

    return 0;
}

void Network::PacketPopMapsFileInfoList(Network::Message & packet, MapsFileInfoList & flist)
{
    flist.clear();
    u16 count;
    packet.Pop(count);
    for(u16 ii = 0; ii < count; ++ii)
    {
	Maps::FileInfo fi;
	PacketPopMapsFileInfo(packet, fi);
	flist.push_back(fi);
    }
}

void Network::PacketPushMapsFileInfo(Network::Message & packet, const Maps::FileInfo & fi)
{
    packet.Push(fi.file);
    packet.Push(fi.name);
    packet.Push(fi.description);
    packet.Push(fi.size_w);
    packet.Push(fi.size_h);
    packet.Push(fi.difficulty);

    packet.Push(static_cast<u8>(KINGDOMMAX));
    for(u8 ii = 0; ii < KINGDOMMAX; ++ii) packet.Push(fi.races[ii]);

    packet.Push(fi.kingdom_colors);
    packet.Push(fi.allow_colors);
    packet.Push(fi.rnd_colors);
    packet.Push(fi.rnd_races);
    packet.Push(fi.conditions_wins);
    packet.Push(fi.wins1);
    packet.Push(fi.wins2);
    packet.Push(fi.wins3);
    packet.Push(fi.wins4);
    packet.Push(fi.conditions_loss);
    packet.Push(fi.loss1);
    packet.Push(fi.loss2);
    packet.Push(static_cast<u32>(fi.localtime));
    packet.Push(static_cast<u8>(fi.with_heroes));
}

void Network::PacketPopMapsFileInfo(Network::Message & packet, Maps::FileInfo & fi)
{
    u8 byte8, race;
    u32 byte32;

    packet.Pop(fi.file);
    packet.Pop(fi.name);
    packet.Pop(fi.description);
    packet.Pop(fi.size_w);
    packet.Pop(fi.size_h);
    packet.Pop(fi.difficulty);

    packet.Pop(byte8);
    for(u8 ii = 0; ii < byte8; ++ii){ packet.Pop(race); fi.races[ii] = race; }

    packet.Pop(fi.kingdom_colors);
    packet.Pop(fi.allow_colors);
    packet.Pop(fi.rnd_colors);
    packet.Pop(fi.rnd_races);
    packet.Pop(fi.conditions_wins);
    packet.Pop(fi.wins1);
    packet.Pop(fi.wins2);
    packet.Pop(fi.wins3);
    packet.Pop(fi.wins4);
    packet.Pop(fi.conditions_loss);
    packet.Pop(fi.loss1);
    packet.Pop(fi.loss2);
    packet.Pop(byte32);
    fi.localtime = byte32;
    packet.Pop(byte8);
    fi.with_heroes = byte8;
}

void Network::PacketPushPlayersInfo(Network::Message & m, const std::vector<FH2RemoteClient> & v, u32 exclude)
{
    u8 count = std::count_if(v.begin(), v.end(), std::not1(std::bind2nd(std::mem_fun_ref(&Player::isID), 0)));
    m.Push(count);
    
    if(count)
    {
	std::vector<FH2RemoteClient>::const_iterator itc1 = v.begin();
	std::vector<FH2RemoteClient>::const_iterator itc2 = v.end();
	for(; itc1 != itc2; ++itc1) if((*itc1).player_id && (*itc1).player_id != exclude)
	{
	    m.Push((*itc1).player_color);
	    m.Push((*itc1).player_race);
    	    m.Push((*itc1).player_name);
	    m.Push((*itc1).player_id);
	}
    }
}

u8 Network::GetPlayersColors(const std::vector<FH2RemoteClient> & v)
{
    u8 res = 0;
    std::vector<FH2RemoteClient>::const_iterator it1 = v.begin();
    std::vector<FH2RemoteClient>::const_iterator it2 = v.end();
    for(; it1 != it2; ++it1) if((*it1).player_id && (*it1).player_color) res |= (*it1).player_color;
                
    return res;
}

#endif
