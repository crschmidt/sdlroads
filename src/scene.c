/******************************************************************************
 *
 *  scene.c -- The controlling logic behind what's on the screen during 
 *             gameplay.  A better name might be `game.c'.
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

#include "core.h"
#include "track.h"
#include "args.h"
#include "camera.h"


void scene_update()
{   
	if(!game_state.paused)
	{
		ship_update(&player);
		camera_update();
	}
}

void scene_init()
{
	ship_init(&player);

	game_state.paused = 0;
	player.vel[0] = 0;

	if(game_state.level >= game_state.track_count)
		game_state.level = 0;

	if(game_state.level >= 0)
	{
		if(!track_load())
		{
			error("Track load error.");
			quit(-4);
		}
		render_init();
	}

}

void set_level(int l)
{
	game_state.level = l;
	track_destroy();
	render_kill();
	scene_init();
}

/* $Id: scene.c,v 1.14 2005/01/12 23:45:13 y3t39 Exp $ */
