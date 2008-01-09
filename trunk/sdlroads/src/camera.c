/******************************************************************************
 *
 *  camera.c -- View control
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

#include "sdlroads.h"

vec3_t right = {1,0.0};
vec3_t up = {0,1,0};

vec3_t camera_look, camera_pos, camera_up;
vec_t camera_dspeed; /* speed in the look direction */
vec_t camera_cspeed; /* speed in the across direction (WRT look) */


static void normalize_vectors();


void camera_init()
{
    camera_pos[0] = CAMERA_X; 
    camera_pos[1] = CAMERA_Y; 
    camera_pos[2] = player.pos[2] + CAMERA_Z_OFFSET;
    
    camera_look[0] = CAMERA_LOOK_X; 
    camera_look[1] = CAMERA_LOOK_Y; 
    camera_look[2] = CAMERA_LOOK_Z;
    CrossProduct(camera_look, right, camera_up);
    normalize_vectors();

}
void camera_update()
{
    camera_pos[2] = player.pos[2] + CAMERA_Z_OFFSET;
}

static void normalize_vectors()
{
    VectorNormalize(camera_look);
    VectorNormalize(camera_up);
}
/* $Id: camera.c,v 1.6 2004/12/31 00:31:23 y3t39 Exp $ */
