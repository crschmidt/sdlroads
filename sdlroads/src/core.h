/******************************************************************************
 *
 *  core.h -- common header for core objects
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

#ifndef __SDL_PROGRAM__CORE_H__
#define __SDL_PROGRAM__CORE_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <stdlib.h>

/* important definitions */
#include "consts.h"
#include "defs.h"
#include "mathlib.h"


/* the core static objects */
#include "settings.h"
#include "window.h"
#include "render.h"
#include "input.h"
#include "scene.h"


#endif
/* $Id: core.h,v 1.3 2004/12/31 00:31:23 y3t39 Exp $ */
