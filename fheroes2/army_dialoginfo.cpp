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
#include "background.h"
#include "button.h"
#include "cursor.h"
#include "config.h"
#include "payment.h"
#include "world.h"
#include "kingdom.h"
#include "animation.h"
#include "localevent.h"
#include "monster.h"
#include "heroes.h"
#include "morale.h"
#include "luck.h"
#include "tools.h"
#include "text.h"
#include "army.h"

Dialog::answer_t Army::Troops::ShowDialogInfo(const Heroes * heroes)
{
    const std::string viewarmy(H2Config::EvilInterface() ? "VIEWARME.ICN" : "VIEWARMY.ICN");

    const Surface & sprite_dialog = AGG::GetICN(viewarmy, 0);

    const Monster::stats_t monster = Monster::GetStats(GetMonster());
    
    Rect pos_rt;

    pos_rt.x = Display::SMALL == display.w() ? 0 : (display.w() - sprite_dialog.w()) / 2;
    pos_rt.y = Display::SMALL == display.w() ? 0 : (display.h() - sprite_dialog.h()) / 2;
    pos_rt.w = Display::SMALL == display.w() ? 640 : sprite_dialog.w();
    pos_rt.h = Display::SMALL == display.w() ? 480 : sprite_dialog.h();

    Background back(pos_rt);
    
    back.Save();

    display.Blit(sprite_dialog, pos_rt);

    Rect dst_pt(pos_rt);
    std::string message;

    // name
    Text(dst_pt.x + 140 - Text::width(monster.name, Font::BIG) / 2, dst_pt.y + 40, monster.name, Font::BIG, true);
    
    // count
    String::AddInt(message, GetCount());
    dst_pt.x = pos_rt.x + 140;
    dst_pt.y = pos_rt.y + 225;
    Text(dst_pt.x - Text::width(message, Font::BIG) / 2, dst_pt.y, message, Font::BIG, true);
    
    // attack
    message = "Attack:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y = pos_rt.y + 40;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message.clear();

    if(heroes)
    {
	message += "(";
	String::AddInt(message, monster.attack + (*heroes).GetAttack());
	message += ")";
    }
    else
	String::AddInt(message, monster.attack);

    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // defense
    message = "Defense:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message.clear();

    if(heroes)
    {
	message += "(";
	String::AddInt(message, monster.defence + (*heroes).GetDefense());
	message += ")";
    }
    else
        String::AddInt(message, monster.defence);

    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // shot
    message = "Shots:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message.clear();
    String::AddInt(message, monster.shots);
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // damage
    message = "Damage:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message.clear();
    String::AddInt(message, monster.damageMin);
    message += " - ";
    String::AddInt(message, monster.damageMax);
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // hp
    message = "Hit Points:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message.clear();
    String::AddInt(message, monster.hp);
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // speed
    message = "Speed:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message = Speed::String(monster.speed);
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // morale
    message = "Morale:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message = (heroes ? "From Heroes" : Morale::String(Morale::NORMAL));
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);

    // luck
    message = "Luck:";
    dst_pt.x = pos_rt.x + 400;
    dst_pt.y += 18;
    Text(dst_pt.x - Text::width(message, Font::BIG), dst_pt.y, message, Font::BIG, true);

    message = (heroes ? "From Heroes" : Luck::String(Luck::NORMAL));
    dst_pt.x = pos_rt.x + 420;
    Text(dst_pt.x, dst_pt.y, message, Font::BIG, true);
    
    
    
    
    //Animation animeMonster(".ICN", 0, 40, Animation::INFINITY | Animation::RING | Animation::LOW);
    
    // button upgrade
    Button *buttonUpgrade = NULL;
    bool upgrade = false;
    dst_pt.x = pos_rt.x + 284;
    dst_pt.y = pos_rt.y + 190;

    if(monster.monster != Monster::Upgrade(monster.monster))
    {
	buttonUpgrade = new Button(dst_pt, viewarmy, 5, 6);
        
	upgrade = world.GetMyKingdom().GetFundsResource() > PaymentConditions::UpgradeMonster(Monster::Upgrade(monster.monster));

	if(! upgrade) (*buttonUpgrade).Press();
    }

    // button dismiss
    bool dismiss = heroes && 1 == (*heroes).GetCountArmy() ? false : true;
    dst_pt.x = pos_rt.x + 284;
    dst_pt.y = pos_rt.y + 222;
    Button buttonDismiss(dst_pt, viewarmy, 1, 2);
    if(! dismiss) buttonDismiss.Press();

    // button exit
    dst_pt.x = pos_rt.x + 410;
    dst_pt.y = pos_rt.y + 222;
    Button buttonExit(dst_pt, viewarmy, 3, 4);
    
    LocalEvent & le = LocalEvent::GetLocalEvent();
    
    display.Flip();
    Cursor::Show();
    
    Dialog::answer_t result = Dialog::ZERO;
    bool exit = false;
        
    // dialog menu loop
    while(! exit)
    {
        le.HandleEvents();

        if(buttonUpgrade) upgrade && le.MousePressLeft(*buttonUpgrade) ? (*buttonUpgrade).Press() : (*buttonUpgrade).Release();
        dismiss && le.MousePressLeft(buttonDismiss) ? buttonDismiss.Press() : buttonDismiss.Release();
        le.MousePressLeft(buttonExit) ? buttonExit.Press() : buttonExit.Release();

        // exit
        if(le.MouseClickLeft(buttonExit) || le.KeyPress(SDLK_ESCAPE)){ result = Dialog::CANCEL; exit = true; }

	//animeMonster.DrawSprite();
    }

    le.ResetKey();

    if(buttonUpgrade) delete buttonUpgrade;

    Cursor::Hide();

    back.Restore();

    return result;
}
