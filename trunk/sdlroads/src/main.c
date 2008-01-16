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

#include "sdlroads.h"

static SDL_TimerID tick_timer_id;
static SDL_Event tick_event;

Uint8 *keydown;

Uint32 timer_tick(Uint32 interval, void *param)
{
	sdl_error_check(SDL_PushEvent(&tick_event) == 0);
	return interval;
}

int main(int argc, char** argv)
{
    SDL_Event event;

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

    sdl_error_check(SDL_Init(SDL_INIT_TIMER) == 0);
	tick_event.type = SDL_USEREVENT;
	sdl_error_check(tick_timer_id = SDL_AddTimer(1000/WINDOW_FPS, timer_tick, NULL));

	keydown = SDL_GetKeyState(NULL);

    while(SDL_WaitEvent(&event))
    {
        switch(event.type)
        {
			case SDL_KEYDOWN:
				// menu input is only interested in keydown events
				if(game_state.state == MENU)
					menu_input(&event.key.keysym);
				break;
            case SDL_QUIT:
                quit(0);
                break;
			case SDL_USEREVENT:
				scene_update();
				window_draw();
				break;
            default:
                break;
        }
    }
	
	/* broke out of event loop */
	sdl_error_check(0);

	/* unreachable */
    return 1;
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
/* $Id$ */
