/******************************************************************************
 *
 *  args.c -- argument parsing functions
 *  Written by Christopher Schmidt <crschmidt@crschmidt.net>
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

#ifndef __ARGS_H
#define __ARGS_H

#include "SDL_opengl.h"
#include "defs.h"

extern int parse_args(int argc, char** argv, game_options_t *config);

#endif
/* $Id: args.h,v 1.8 2004/12/31 00:31:23 y3t39 Exp $ */
