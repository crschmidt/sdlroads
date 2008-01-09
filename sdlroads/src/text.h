/******************************************************************************
 *
 *  text.h -- functions for outputting text to the window
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

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "args.h"


extern void text_init();
extern void text_kill();
extern void print_large(const char* string, Uint16 start_x, Uint16 start_y);
extern void print_small(const char* string, Uint16 start_x, Uint16 start_y);
extern void print_large_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y);
extern void print_small_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y);
/* $Id: text.h,v 1.9 2004/12/31 00:31:24 y3t39 Exp $ */
