/******************************************************************************
 *
 *  defs.h -- global definitions
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

#ifndef __SDL_PROGRAM__DEFS_H__
#define __SDL_PROGRAM__DEFS_H__

#include "SDL.h"
#include "SDL_opengl.h"
#include "mathlib.h"

/* call this function to gracefully quit the game */
extern void quit(int code);


/* generic structures */
typedef struct
{
    Uint16 x,y;
} coord_t;


/* game status data
 */
typedef struct
{
    SDL_bool fullscreen;
    SDL_bool music_on;
    SDL_bool sound_on;

    GLushort width;
    GLushort height;
    GLfloat aspect_ratio;

    char musicfilename[256];
    char modelname[256];

    char track_path[256];

} game_options_t;


typedef enum
{
    PLAY,
    MENU
} state_enum;

typedef struct
{
    state_enum state;
    int level;
    int paused;
    int victory;
    SDL_Surface *window;
    int window_valid;
    GLuint bg_tex_num;
    int track_count;
    char **track_list;
} game_state_t;


/* structs for mesh data
 */
typedef struct
{
    char name[90];
    GLfloat shininess, alpha;
    GLfloat k_diffuse[3];
    GLfloat k_ambient[3];
    GLfloat k_specular[3];
} material_t;

typedef struct
{
    GLushort mat_index;
    GLushort vertex_count;
    GLuint* v_indices;          /* parallel */
    GLuint* vn_indices;         /* arrays ! */
}face_t;

typedef struct
{
    GLushort mat_count;
    material_t* materials;

    GLuint face_count;
    face_t* faces;

    GLuint vertex_count;
    vec3_t* vertices;

    GLuint normal_count;
    vec3_t* normals;

    GLfloat explosion_frame;

} mesh_t;


/* ship data 
 */
typedef struct
{
    vec3_t vel;

    GLfloat ship_jump_v0;

    vec3_t pos;
    vec3_t front_r, back_r;
    vec3_t front_l, back_l;
    vec3_t top, bottom;

    mesh_t model;
    GLuint display_list;

    GLubyte* texture;
    GLushort tex_w, tex_h;
    GLuint tex_num;

    /* model correction */
    GLfloat x_scale, y_scale, z_scale;
    GLfloat x_shift, y_shift, z_shift;

    GLboolean on_ground;

    GLboolean exploding;
    GLfloat collision_speed;

    GLboolean initialized;

    /* prevents position from being erased on a ship_init */
    GLboolean save;

} ship_t;

/* information about the track 
 */
typedef struct
{
    GLubyte *image_data;
    GLushort height, width;
} imagedata_t;

imagedata_t image;
        

typedef struct
{
    GLushort color;
    GLfloat top, left, right, front, back;
    char height;
} trackdata_t;

typedef struct
{
    trackdata_t **track_rows;
    int num_positions;
    int high_extra_color;
    float grav;
    char bg_filename[256];
} track_t;

game_state_t game_state;
game_options_t settings;
ship_t player;
track_t track;

/* macros */
#define str(a) #a
#define xstr(b) str(b)
#define error(s) error_print("[" xstr(__FILE__) ", line " xstr(__LINE__)"] ", s)

#define CLAMP(n, l, h) if(n<l){n=l;} if(n>h){n=h;} 


#endif
/* $Id$ */
