/******************************************************************************
 *
 *  sound.c -- sound support
 *  Written by Paul Guder <enyalios@enyalios.net>
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

#ifndef NO_SOUND

#include "SDL_mixer.h"

static Mix_Music *music;
static Mix_Chunk *explosion;

void sound_init()
{
    char crash_file[256];
	char music_file[256];

    if(SDL_Init(SDL_INIT_AUDIO) < 0) 
    {
        printf("Unable to init audio: %s\n", SDL_GetError());
        exit(1);
    }

    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
    { 
        printf("Warning: Couldn't set 44100 Hz 16-bit audio\n- Reason: %s\n",
                SDL_GetError());
    }

    /* start the music, if needed */
    if(*settings.musicfilename)
    {
		prepend_path(SOUND_DIR, settings.musicfilename, music_file);
        music = Mix_LoadMUS(music_file);
		if(music == NULL)
		{
			error("Couldn't load music: ");
			error(SDL_GetError());
		}
    }

	music_toggle(settings.music_on);

    /* load the crash sound */
    prepend_path(SOUND_DIR, "game_crash.ogg", crash_file);
    explosion = Mix_LoadWAV(crash_file);
}


/* these functions are called by the options menu */

void music_toggle(int onoff)
{
	int result;

	if(onoff)
		result = Mix_FadeInMusic(music, -1, 1500);
	else
		result = Mix_FadeOutMusic(1500); 

	if(result == -1)
	{
		error("Well, that music toggle failed: ");
		error(SDL_GetError());
	}
		

	settings.music_on = onoff;
}

void sound_toggle(int onoff)
{
	settings.sound_on = onoff;
}


void crash_sound()
{
	if(settings.sound_on)
		Mix_PlayChannel(1,explosion,0);
}

void sound_kill()
{
    Mix_FreeMusic(music);
    Mix_FreeChunk(explosion);
}

#else // NO_SOUND

void sound_init() {}
void crash_sound(){}
void sound_kill() {}
void music_toggle(int onoff) {}
void sound_toggle(int onoff) {}

#endif

/* $Id$ */
