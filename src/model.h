/******************************************************************************
 *
 *  model.h -- Model loading functions
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

#include "defs.h"

#ifndef __MODEL_H__
#define __MODEL_H__



/* name has no path or file extension in this function */
extern GLuint mesh_cache(char* name, mesh_t* mesh);
extern void destroy_mesh(mesh_t* mesh);
extern void mesh_dump(mesh_t* mesh);
extern void render_exploding(mesh_t* mesh);



#endif
/* $Id: model.h,v 1.15 2004/12/31 00:31:23 y3t39 Exp $ */