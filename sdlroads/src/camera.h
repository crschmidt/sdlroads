/******************************************************************************
 *
 *  camera.h -- View control
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

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "mathlib.h"

vec3_t camera_look, camera_pos, camera_up;
vec_t camera_dspeed; /* speed in the look direction */
vec_t camera_cspeed; /* speed in the across direction (WRT look) */

extern void camera_spin(float angle);
extern void camera_tilt(float angle);
extern void camera_update();

#endif
/* $Id: camera.h,v 1.6 2004/12/31 00:31:23 y3t39 Exp $ */
