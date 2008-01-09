/******************************************************************************
 *
 *  main.c -- entry point and event loop
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
#include "args.h"
#include "sound.h"
#include "menu.h"

/* one-time init function - contains a set of actions to be 
 * performed only ONCE per run */
static void init(int argc, char** argv)
{
    settings_init(argc, argv);

    track_list_read();
    menu_init();
    window_init(0);
    sound_init();
    text_init();

    player.initialized = GL_FALSE;
    player.save = GL_FALSE;

    game_state.state = MENU;
    game_state.paused = 0;
    game_state.level = -1;
    game_state.victory = 0;
    game_state.window_valid = 1;

    render_init();

    SDL_Init(0);
}

int main(int argc, char** argv)
{
    SDL_Event event;
    init(argc, argv);

    while(1)
    {
        SDL_PumpEvents();

        while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
        {
            switch(event.type)
            {
                case SDL_KEYUP:
                    if(game_state.state != MENU)
                        input_keyup(&event.key.keysym);

                    break;
                case SDL_KEYDOWN:
                    if(game_state.state == MENU)
                        menu_input(&event.key.keysym);
                    else
                        input_keydown(&event.key.keysym);

                    break;
                case SDL_QUIT:
                    quit(0);
                    break;
                default:
                    break;
            }
        }
        window_draw();
    }

    /* never happens */
    return -9;
}

void quit(int code)
{
    puts("Exiting.");
    fflush(stdout);
    track_destroy();
    text_kill();
    sound_kill();
    menu_destroy();
    track_list_release();
    SDL_Quit();
    if(code == 0)
        settings_file_write();
    exit(0);
}
/* $Id: main.c,v 1.24 2005/01/12 22:38:01 y3t39 Exp $ */
