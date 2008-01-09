/******************************************************************************
 *
 *  util.h -- miscellaneous functions
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

#ifndef __UTIL_H__
#define __UTIL_H__

extern void prepend_path(const char* dir, const char* file, char* s);
extern void prepend_config_path(const char *file, char *s);
extern void toggle_fullscreen(int on_off);
extern int res_code_width(int res_code);
extern int res_code_height(int res_code);
extern int width_res_code(int width);
extern void toggle_resolution(int res_code);

#endif
/* $Id: util.h,v 1.4 2004/12/31 00:31:24 y3t39 Exp $ */
