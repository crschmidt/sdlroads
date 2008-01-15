/******************************************************************************
 *
 *  util.c -- miscellaneous functions
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


/* FIXME: it would be best if this were a macro */
void prepend_path(const char* dir, const char* file, char* s)
{
    s[0] = '\0';
    strcat(s, PATH_PREFIX);
    strcat(s, dir);
    strcat(s, file);
}

/* FIXME: this too */
void prepend_config_path(const char *file, char *s)
{
    s[0] = '\0';
    strcat(s, (const char*)CONFIG_DIR);
    strcat(s, "/");
    strcat(s, file);
}

void toggle_fullscreen(int on_off)
{
    if(on_off == -1)
    {
        settings.fullscreen = !settings.fullscreen;
        on_off = settings.fullscreen;
    }
    else
        settings.fullscreen = on_off;

    game_state.window_valid = 0;
}

/* functions mapping values from 0 on up to meaningful sets of values
 * describing game settings. */
int res_code_width(int res_code)
{
    switch(res_code)
    {
        case 0: return 1280; break;
        case 1: return 1024; break;
        case 2: return 800; break;
    }
    return 640;
}

int width_res_code(int width)
{
    switch(width)
    {
        case 1280: return 0; break;
        case 1024: return 1; break;
        case 800: return 2; break;
    }
    return 3;
}

int res_code_height(int res_code)
{
    switch(res_code)
    {
        case 0: return 1024; break;
        case 1: return 768; break;
        case 2: return 600; break;
    }
    return 480;
}

void toggle_resolution(int res_code)
{
    settings.width = res_code_width(res_code);
    settings.height = res_code_height(res_code);

    game_state.window_valid = 0;
}

/* logging functions */

typedef enum
{
    RESET,
    BOLD,

    TEAL,
    TURQUOISE,

    FUCHSIA,
    PURPLE,

    BLUE,
    DARKBLUE,

    GREEN,
    DARKGREEN,

    YELLOW,
    BROWN,

    RED,
    DARKRED
} stdout_color_enum;

static int 
cprintf(stdout_color_enum color, const char * format, ...)
{
    va_list ap;
    int rval;

    const char color_codes[][20] = 
    {
       "\x1b[0m",
       "\x1b[01m",

       "\x1b[36;06m",
       "\x1b[36;01m",

       "\x1b[35;01m",
       "\x1b[35;06m",

       "\x1b[34;01m",
       "\x1b[34;06m",

       "\x1b[32;01m",
       "\x1b[32;06m",

       "\x1b[33;01m",
       "\x1b[33;06m",

       "\x1b[31;01m",
       "\x1b[31;06m"
    };

    va_start(ap, format);
#ifndef WIN32
    printf(color_codes[color]);
#endif
    rval = vprintf(format, ap);
#ifndef WIN32
    printf(color_codes[RESET]);
#endif

    va_end(ap);

    return rval;
}

void
error_print(const char * where, const char * str)
{
    /* stdout logging */
    cprintf(RED, "ERROR! ");
    cprintf(BLUE, where);
    putchar(' ');
    cprintf(YELLOW, str);
    printf("\n");

    /* TODO: in game logging, perhaps? */
}

void sr_message(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void _sr_fatal(const char *msg, const char *file, int line)
{
	sr_message("SDL_Error in %s, line %d: %s\n", file, line, msg);
	quit(1);
}

void _sr_assert(const char *string, const char *file, int line)
{
	sr_message("Assertion failed: \"%s\" in %s, line %d\n", string, file, line);
	quit(1);
}

/* $Id$ */
