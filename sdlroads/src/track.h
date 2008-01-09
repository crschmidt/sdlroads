/******************************************************************************
 *
 *  track.h -- track defs
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

#include "SDL_opengl.h"
#include "mathlib.h"

#ifndef __TRACK_H__
#define __TRACK_H__

#define track_height_difference(a,b)  \
    track_height_at_point(a)-track_height_at_point(b)


extern GLboolean track_load();
extern void track_render(GLushort start, GLushort stop);
extern void track_destroy();
extern GLfloat track_height_at_point(vec3_t point);
extern GLshort track_i(vec3_t point);
extern GLshort track_j(vec3_t point);
extern trackdata_t* track_element_at_point(vec3_t point);
extern GLboolean track_height_increase(vec3_t a, vec3_t b);
extern GLfloat track_to_point_dist(vec3_t p);
extern GLboolean point_off_ground(vec3_t p);
extern GLfloat track_to_ship(vec3_t p);
extern void track_list_read();
extern void track_list_release();


#endif
/* $Id: track.h,v 1.8 2004/12/31 00:31:24 y3t39 Exp $ */
