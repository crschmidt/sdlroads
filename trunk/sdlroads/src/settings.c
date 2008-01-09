/******************************************************************************
 *
 *  settings.c -- game configuration routines
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
#include <ctype.h>


static GLushort width;

int settings_file_write_default()
{
    FILE *f;
    char filename[256];

    prepend_config_path(CONFIG_FILE, filename);
    f = fopen(filename, "w");

    if(f == NULL)
        return 0;

    printf("[settings] writing a default configuration file: %s\n",
            filename);

    fprintf(f, "width %d\n", DEFAULT_WIDTH);
    fprintf(f, "height %d\n", DEFAULT_HEIGHT);
    fprintf(f, "fullscreen %d\n", DEFAULT_FULLSCREEN);
    fprintf(f, "music %d\n", DEFAULT_MUSIC_TOGGLE);
	fprintf(f, "sound %d\n", DEFAULT_SOUND_TOGGLE);

    if(fclose(f) != 0)
    {
        fprintf(stderr, "Error wrting default configuration file!\n");
        return 0;
    }
    return 1;
}

int settings_file_read()
{
    char filename[256];
    char buf[100];
    FILE* f;
    unsigned short line = 0;
    int scanreturn;

    prepend_config_path(CONFIG_FILE, filename);

#ifdef WIN32

    f = fopen(filename, "r");
	if (!f)
		return 1;

#else
    if(access(filename, F_OK) == 0)
    {
        printf("Attempting to open file: %s\n", filename);
        f = fopen(filename, "r");
    }
    else if(settings_file_write_default())
        f = fopen(filename, "r");
    else
    {
        fprintf(stderr, "Unable to write a configuration file\n");
        return;
    }
#endif

    while(fgets(buf, 100, f))
    {
        line++;
        scanreturn = 1;

        if(isspace(buf[0]))
            continue;

        switch(buf[0])
        {
            case 'h':
                scanreturn = sscanf(buf, "height %hu\n", &settings.height);
                break;
            case 'w':
                scanreturn = sscanf(buf, "width %hu\n", &settings.width);
                break;
            case 'm':
                scanreturn = sscanf(buf, "music %d\n", &settings.music_on);
                break;
			case 's':
				scanreturn = sscanf(buf, "sound %d\n", &settings.sound_on);
				break;
            case 'f':
                scanreturn = sscanf(buf, "fullscreen %i\n", &settings.fullscreen);
                break;
            case 't':
                scanreturn = sscanf(buf, "trackpath %s\n", &settings.track_path);
                break;
        }

        if(scanreturn == 0)
            fprintf(stderr, "Error reading line %i in %s: %s\n",
                    line, CONFIG_FILE, line);
    }

    fclose(f);

    return 1;
}

int settings_file_write()
{
    FILE *f;
    char filename[256];

    prepend_config_path(CONFIG_FILE, filename);
    f = fopen(filename, "w");

    if(f == NULL)
        return 0;

    printf("[settings] writing config file: %s\n",
            filename);

    fprintf(f, "width %d\n", settings.width);
    fprintf(f, "height %d\n", settings.height);
    fprintf(f, "fullscreen %d\n", settings.fullscreen);
    fprintf(f, "music %d\n", settings.music_on);
    fprintf(f, "sound %d\n", settings.sound_on);

    if(fclose(f) != 0)
    {
        fprintf(stderr, "Error wrting configuration file!\n");
        return 0;
    }
    return 1;
}

/* returns zero if the game should not run */
int settings_init(int argc, char **argv)
{
    /* set defaults
     */
    game_state.state = DEFAULT_GAMESTATE;
    settings.width = DEFAULT_WIDTH;
    settings.height = DEFAULT_HEIGHT;
    settings.fullscreen = DEFAULT_FULLSCREEN;
	settings.music_on = DEFAULT_MUSIC_TOGGLE;
	settings.sound_on = DEFAULT_SOUND_TOGGLE;
    sprintf(settings.musicfilename, DEFAULT_MUSICFILENAME);
    strcpy(settings.modelname, DEFAULT_MODELNAME);
    strcpy(settings.track_path, TRACK_DIR);

    /* replace settings with config file entries 
     */

    settings_file_read();

    /* finally, command line arguments replace all
     */

    if(!parse_args(argc, argv, &settings))
        exit(1);

    printf("configuration summary:\n"
           "========================\n"
           "screen size: %hux%hu\n"
           "music file: %s\n"
		   "music on: %d\n"
		   "sound on: %d\n"
           "model: %s\n"
           "========================\n",
            settings.width,
            settings.height,
            settings.musicfilename,
			settings.music_on,
			settings.sound_on,
            settings.modelname);

    settings.aspect_ratio =
        (GLfloat)settings.width / 
        (GLfloat)settings.height;

	return 1;
}
/* $Id$ */
