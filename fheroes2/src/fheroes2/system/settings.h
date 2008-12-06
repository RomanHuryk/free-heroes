/***************************************************************************
 *   Copyright (C) 2008 by Andrey Afletdinov                               *
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

#ifndef H2SETTINGS_H
#define H2SETTINGS_H

#include <string>
#include <iostream>
#include "gamedefs.h"
#include "difficulty.h"
#include "maps_fileinfo.h"
#include "game.h"
#include "config.h"

class Settings
{
public:

    typedef enum
    {
	NONE              = 0x00000000,

	PREFERABLY_COUNT2 = 0x00000002,
	PREFERABLY_COUNT3 = 0x00000003,
	PREFERABLY_COUNT4 = 0x00000004,
	PREFERABLY_COUNT5 = 0x00000005,
	PREFERABLY_COUNT6 = 0x00000006,

        SOUND             = 0x00010000,
        FULLSCREEN        = 0x00020000,
        LOGO              = 0x00040000,
        EDITOR            = 0x00080000,
        MUSIC_EXT         = 0x00100000,
        MUSIC_CD          = 0x00200000,
        MUSIC_MIDI        = 0x00400000,
        ORIGINAL          = 0x00800000,
        PRICELOYALTY      = 0x01000000,
        EVILINTERFACE     = 0x02000000,
        SHADOW            = 0x04000000,
        BATTLEGRID        = 0x08000000,
        BATTLEMOUSESHADOW = 0x10000000,
        BATTLEMOVESHADOW  = 0x20000000,

	MUSIC		= MUSIC_CD | MUSIC_EXT | MUSIC_MIDI,

    } settings_t;

    static Settings & Get(void);

    bool Read(const std::string & filename);
    bool Save(const std::string & filename) const;

    void Dump(std::ostream & stream = std::cout) const;
    void LoadFileMaps(const std::string & file);

    u8 MajorVersion(void) const;
    u8 MinorVersion(void) const;
    u32 DateBuild(void) const;
    u8 Debug(void) const;
    u8 Animation(void) const;

    const Maps::FileInfo & FileInfo(void) const;
    Maps::FileInfo & FileInfo(void);

    const std::string & TranslationFile(void) const;
    const std::string & DataDirectory(void) const;
    const std::string & MapsDirectory(void) const;

    Difficulty::difficulty_t GameDifficulty(void) const;

    bool Editor(void) const;
    bool Original(void) const;
    bool Sound(void) const;
    bool Music(void) const;
    bool FullScreen(void) const;
    bool EvilInterface(void) const;
    bool Shadow(void) const;
    bool Logo(void) const;
    bool BattleGrid(void) const;
    bool BattleMovementShaded(void) const;
    bool BattleMouseShaded(void) const;

    bool Modes(const settings_t s) const;

    const Size & VideoMode(void) const;

    u8   SoundVolume(void) const;
    u8   MusicVolume(void) const;

    void SetDebug(const u8 d);
    void SetGameDifficulty(const Difficulty::difficulty_t d);
    void SetModes(const settings_t s);
    void ResetModes(const settings_t s);

    void SetSoundVolume(const u8 v);
    void SetMusicVolume(const u8 v);
    void SetAnimation(const u8 s);
    
    u8   GameType(void) const;
    void SetGameType(const Game::type_t type);

    Color::color_t MyColor(void) const;
    void SetMyColor(const Color::color_t c);
    u8   Players(void) const;
    void SetPlayers(u8 c);
    u8   PreferablyCountPlayers(void) const;
    void SetPreferablyCountPlayers(u8 c);

protected:
    void Parse(const std::string & left, const std::string & right);
    void SetModes(const std::string & key);
    void ResetModes(const std::string & key);

private:
    Settings();

    const u8 major_version;
    const u8 minor_version;
    const u32 build_date;

    u32 modes;
    u8 debug;

    Size video_mode;

    Difficulty::difficulty_t game_difficulty;
    Color::color_t my_color;

    std::string path_data_directory;
    std::string path_maps_directory;
    
    std::string translationFile;
    
    Maps::FileInfo current_maps_file;
    
    u8 sound_volume;
    u8 music_volume;
    u8 animation;

    u8 game;
    u8 players;
    u8 preferably_count_players;
};

#endif
