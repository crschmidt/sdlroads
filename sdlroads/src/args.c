/******************************************************************************
 *
 *  args.c -- argument parsing functions
 *  Written by Christopher Schmidt <crschmidt@crschmidt.net>
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

#include <string.h>


static void print_help(char **argv);
static void print_version(char **argv);


int parse_args(int argc, char** argv, game_options_t *config) {

    int i, returnval;

    returnval = 1;

    for (i=1; i<argc; i++) {
        switch (argv[i][1]) {
            case 'f':
                config->fullscreen = 1;
                break;
            case 'h':
                config->height = atoi(argv[i+1]);
                i++;
                break;
            case 'w':
                config->width = atoi(argv[i+1]);
                i++;
                break;
            case 'm':
                strcpy(config->musicfilename, argv[i+1]);
                i++;
                break;
            case 't':
                strcpy(config->track_path, argv[i+1]);
                i++;
                break;
            case 'v':
            case 'V':
                print_version(argv);
				exit(0);
                break;
            case '?':
            default: 
                print_help(argv);
                returnval = 0;
                break;

        }
    }

    return returnval;
}

void print_help(char **argv) 
{
    printf("Syntax: %s [options]\n", argv[0]);
    printf("\t-f\t\tfullscreen\n");
    printf("\t-w n\t\twidth\n");
    printf("\t-h n\t\theight\n");
    printf("\t-m filename\tmusic filename\n");
    printf("\t-v\t\tversion info\n");
    printf("\t-t\t\ttrack directory\n");
    printf("\t-?\t\tthis message\n");
}

void print_version(char **argv) 
{
    char version_string[50] = "This is sdlroads, version ";
    strcat(version_string, SDLROADS_VERSION);
    strcat(version_string, "\n");
    printf(version_string);
}

/* $Id$ */
