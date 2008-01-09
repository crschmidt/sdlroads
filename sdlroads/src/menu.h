/******************************************************************************
 *
 *  menu.h -- menu definitions
 *
 *  This file is part of SDLRoads
 *  Copyright (C) 2004 Peter Newman <panewman@uiuc.edu>.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****************************************************************************/

#ifndef __MENU_H__
#define __MENU_H__

#include "mathlib.h"
#include "defs.h"


typedef enum
{
    IT_LEVEL,       /* sticks the player in a level */
    IT_ZAP,         /* zaps to another menu */
    IT_OPTION       /* item with options to choose from */
} itemtype_enum;


/* Menu structures
 */

typedef struct
{
    int optioncount;
    char **option_headings;
    int selected_option;

    /* function to call when setting is changed */
    void (*option_change_callback)(int selected);
} option_data_t;

typedef struct
{
    char text[32];
    itemtype_enum type;
    void *itemdata;
} menuitem_t;

struct menu_tag
{
    char title[32];
    struct menu_tag *parent;

    menuitem_t *items;
    int itemcount;
    int selected;
};
typedef struct menu_tag menu_t;


extern void menu_init();
extern void menu_render();
extern void menu_input(SDL_keysym *key);
extern void menu_destroy();

#endif
/* $Id: menu.h,v 1.10 2004/12/31 00:31:23 y3t39 Exp $ */
