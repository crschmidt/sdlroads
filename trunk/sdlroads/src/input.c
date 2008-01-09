/******************************************************************************
 *
 *  input.c -- input handling
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

void input_keydown( SDL_keysym* key )
{
    switch(key->sym)
    {
        case SDLK_q:
            quit(0);
            break;

        case SDLK_f:
            toggle_fullscreen(-1);
            break;

        case SDLK_UP:
            if(player.acc[2] == 0)
                player.acc[2] = SHIP_ACCEL;
            break;
        case SDLK_DOWN:
            if(player.acc[2] == 0)
                player.acc[2] = -SHIP_ACCEL;
            break;
        case SDLK_LEFT:
                player.vel[0] += SHIP_CROSS_VEL;
            break;
        case SDLK_RIGHT:
                player.vel[0] -= SHIP_CROSS_VEL;
            break;

        case SDLK_r:
            ship_restart(&player);
            break;

        case SDLK_SPACE:
            if(!game_state.paused)
                ship_jump(&player);
            break;
            
        case SDLK_p:
            game_state.paused = !game_state.paused;
            scene_update();
            render_draw();
            break;

        case SDLK_ESCAPE:
            game_state.state = MENU;
            break;
            
        default:break;
    }
    switch(key->mod)
    {
        case KMOD_LCTRL:
            ship_jump(&player);
            break;
        default:break;
    }
}

void input_keyup( SDL_keysym* key )
{
    switch(key->sym)
    {
        case SDLK_UP:
            if(player.acc[2] == SHIP_ACCEL)
                player.acc[2] = 0;
            break;
        case SDLK_DOWN:
            if(player.acc[2] == -SHIP_ACCEL)
                player.acc[2] = 0;
            break;
        case SDLK_LEFT:
                player.vel[0] -= SHIP_CROSS_VEL;
            break;
        case SDLK_RIGHT:
                player.vel[0] += SHIP_CROSS_VEL;
            break;

        default:break;
    }
}
/* $Id$ */
