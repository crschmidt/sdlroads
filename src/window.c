/******************************************************************************
 *
 *  window.c -- control of the SDL window (or the whole screen)
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
#include "track.h"
#include "text.h"

#include <unistd.h>

int window_init(int re_initialize)
{
    char caption[50] = "SDLroads, v";
    int channel_size = 8;
    Uint32 video_flags;

    strcat(caption, VERSION);

    if(re_initialize)
    {
        if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            fprintf(stderr, "Video didn't make it back up: %s\n", 
                    SDL_GetError());
            quit(-1);
        }
    }
    else
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            fprintf(stderr, "Video initialization failed: %s\n", 
                    SDL_GetError());
            quit(-2);
        }
    }

    SDL_WM_SetCaption(caption, NULL);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    video_flags = SDL_RESIZABLE|SDL_ANYFORMAT|SDL_OPENGL;
    if(settings.fullscreen)
        video_flags |= SDL_FULLSCREEN;

    /* 32-bit should be fine for anybody. */
    game_state.window = 
        SDL_SetVideoMode(settings.width, settings.height, 32, video_flags);

    if(game_state.window == 0)
    {
        fprintf(stderr, "Video mode set (%hu,%hu) failed: %s\n", 
                settings.width, settings.height, SDL_GetError());
        quit(-3);
    }

    SDL_ShowCursor(SDL_DISABLE);

    return 1;
}



void window_draw()
{
    static int last_tick = 0;
    int frame_time = 0;

    if(!game_state.window_valid)
    {
        window_restart();
        game_state.window_valid = 1;
    }

    if(SDL_GetTicks() - last_tick > WINDOW_FPS)
    {
        last_tick = SDL_GetTicks();

        if(game_state.state == MENU)
            menu_render();
        else if(game_state.victory) {
            if(SDL_GetTicks() > game_state.victory + 1000)
            {
                game_state.victory = 0;
				set_level(game_state.level+1);
            }
        }
        else if(!game_state.paused)
        {
            scene_update();
            render_draw();
        } else {
            SDL_Delay(100);
        }
        SDL_GL_SwapBuffers();
        
        frame_time = SDL_GetTicks() - last_tick;
        
        if(frame_time < 20) 
            SDL_Delay(20 - frame_time);
    }
}

void window_restart()
{
    text_kill();
    render_kill();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    track_destroy();
    player.save = GL_TRUE;
    ship_uninitialize(&player);
    window_init(1);
    scene_init();
    render_init();
    text_init();
} 
/* $Id: window.c,v 1.31 2005/01/12 23:45:13 y3t39 Exp $ */
