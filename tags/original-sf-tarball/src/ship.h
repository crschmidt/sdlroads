/******************************************************************************
 *
 *  ship.h -- ship-related defs
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

#include "mathlib.h"
#include "consts.h"
#include "defs.h"
#include "SDL_opengl.h"

#ifndef __SHIP_H__
#define __SHIP_H__

extern void ship_init(ship_t* s);
extern void ship_render(ship_t* s);
extern void ship_update(ship_t* s);
extern void ship_jump(ship_t* s);
extern void ship_die(ship_t* s, int cause);
extern void ship_restart(ship_t* s);
extern void ship_win(ship_t* s);
extern void ship_uninitialize(ship_t *s);
extern inline void ship_sides(ship_t* s);

#endif
/* $Id: ship.h,v 1.10 2004/12/31 00:31:24 y3t39 Exp $ */
