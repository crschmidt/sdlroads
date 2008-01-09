/******************************************************************************
 *
 *  track.c -- track implementation and helper functions
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

#define track_height_difference(a,b)  \
    track_height_at_point(a)-track_height_at_point(b)

/* this will be the number of colors statically defined */
#define STATIC_COLORS 8

static GLfloat color_table[][4] = 
{
    {0.8,0,0,1}, // 0 red
    {0,0.8,0,1}, // 1 green
    {0,0,0.8,1}, // 2 blue
    {0,0.8,0.8,1}, // 3 cyan
    {0.8,0,0.8,1}, // 4 magenta
    {0.8,0.8,0,1}, // 5 yellow
    {0.8,0.8,0.8,1}, // 6 white
    {0,0,0,1} // 7 black
};

/* these colors are defined in the individual track files */
static GLfloat ** extra_colors;

/* helpers for allocating and de-allocating the 2-dimensional dynamic
 * extra_colors array */
static void allocate_extra_colors();
static void free_extra_colors();

/* wrapper for the color arrays */
static GLfloat * get_color(int i);

GLboolean track_load()
{
    GLushort i,j,grav_temp;
    GLfloat total_width, xstart, lbase;
    GLfloat color_tmp[4];
    GLubyte current_height;
    char buf[80];

    FILE* fp = fopen(game_state.track_list[game_state.level], "r");
    if(!fp)
    {
        fprintf(stderr, "Failed to load track file, \"%s\" !\n", 
                game_state.track_list[game_state.level]);
        return 0;
    }

    fgets(buf, 80, fp);
    sscanf(buf, "%hu", &grav_temp);
    track.grav = 400.0f/1000.0f/1000.0f;
    track.grav *= SHIP_GRAV;

    /* oddly enough, the track gravity has a great deal more
     * influence on the ship's y'(0) than the environment's
     * gravity ... */

    player.ship_jump_v0 = 200.0f/((GLfloat)grav_temp + 700.0f);
    /* since the jump function is inversely proportional  ^^
     * we don't want to be too close to the origin... */
    printf("Track grav: %hu\n", grav_temp);

    fgets(buf, 80, fp);
    prepend_path(IMAGE_DIR, buf, track.bg_filename);
    printf("Background File: %s\n", track.bg_filename);
    bg_init(track.bg_filename);
    
    /* intial data-count of the track file so we know how much dynamic
     * memory to request */
    track.num_positions = 0;
    track.high_extra_color = 0;
    while(fgets(buf, 80, fp))
    {
        if(islower(buf[0]))
            track.num_positions++;
        if(buf[0] == 'C')
        {
            sscanf(buf, "COLOR %d (%*f,%*f,%*f,%*f)", &i, 
                    NULL, NULL, NULL, NULL);
            if(i < STATIC_COLORS)
            {
                fprintf(stderr, "Error in track file --"
                        " requested color entry %d not available\n", i);
                return GL_FALSE;
            }
            if(i > track.high_extra_color)
                track.high_extra_color = i;
        }
    }

    /* start on line 3... */
    rewind(fp);
    fgets(buf, 80, fp);
    fgets(buf, 80, fp);

    track.track_rows = 
        (trackdata_t**)malloc(track.num_positions*sizeof(trackdata_t*));
    
    allocate_extra_colors();

    if(!track.track_rows)
    {
        fprintf(stderr,"track.track_rows is null!\n");
        fclose(fp);
        return 0;
    }

    for(i=0; i<track.num_positions; i++)
    {
        track.track_rows[i] = 
            (trackdata_t*)malloc(NUM_LANES*sizeof(trackdata_t));
        if(!track.track_rows[i])
        {
            perror("track row allocation");
            fclose(fp);
            return 0;
        }
    }

    i=0;
    while(fgets(buf, 80, fp) != NULL)
    {
        if(buf[0] == '#' || buf[0] == '\n' || buf[0] == ' ')
            continue;

        if(buf[0] == 'C')
        {
            sscanf(buf, "COLOR %d (%f,%f,%f,%f)", &j, 
                    &color_tmp[0], &color_tmp[1], &color_tmp[2], &color_tmp[3]);
            memcpy(extra_colors[j - STATIC_COLORS], color_tmp, 
                    4*sizeof(GLfloat));
            continue;
        }

        sscanf(buf, "%1c%hu %1c%hu %1c%hu %1c%hu %1c%hu %1c%hu %1c%hu %1c%hu %1c%hu",
                &track.track_rows[i][0].height,&track.track_rows[i][0].color,
                &track.track_rows[i][1].height,&track.track_rows[i][1].color,
                &track.track_rows[i][2].height,&track.track_rows[i][2].color,
                &track.track_rows[i][3].height,&track.track_rows[i][3].color,
                &track.track_rows[i][4].height,&track.track_rows[i][4].color,
                &track.track_rows[i][5].height,&track.track_rows[i][5].color,
                &track.track_rows[i][6].height,&track.track_rows[i][6].color,
                &track.track_rows[i][7].height,&track.track_rows[i][7].color,
                &track.track_rows[i][8].height,&track.track_rows[i][8].color);
        i++;
    }


    total_width = NUM_LANES*LANE_WIDTH;
    xstart = total_width * -0.5;

    printf("track length: %d\n", track.num_positions);
    for(i=0; i<track.num_positions; i++)
    {
        lbase = xstart;
        for(j=0; j<NUM_LANES; j++)
        {
            if(track.track_rows[i][j].height == 'x')
                track.track_rows[i][j].top = -50.0f;
            else
            {
                current_height = track.track_rows[i][j].height - 'a';
                track.track_rows[i][j].top = 
                    TRACK_BASE + 
                    TRACK_BLOCK_BASE + 
                    current_height*TRACK_BLOCK_HEIGHT;
            }

            track.track_rows[i][j].left = lbase + j*LANE_WIDTH;
            track.track_rows[i][j].right = lbase + (j+1)*LANE_WIDTH;
            track.track_rows[i][j].front = i*POSITION_LENGTH; 
            track.track_rows[i][j].back = (i+1)*POSITION_LENGTH; 


            /*printf("%1c%hu ",
                    track.track_rows[i][j].height,
                    track.track_rows[i][j].color);*/
        }
        /*printf("\n");*/


    }

    fclose(fp);

    return 1;
}

void track_render(GLushort start, GLushort stop)
{
    GLushort i,j;
    trackdata_t* b;

    if(stop >= track.num_positions)
        stop = track.num_positions;

    for(i=start; i<stop; i++)
    {

        for(j=0; j<NUM_LANES; j++)
        {
            /* save typing */
            b = &track.track_rows[i][j];

            if(track.track_rows[i][j].height == 'x')
                continue;


            glColor4fv(get_color(track.track_rows[i][j].color));

            glBegin(GL_QUADS);

            /* frontmost face */
            glNormal3f(0, 0, -1);
            glVertex3f(b->left,
                       b->top,
                       b->front);
            glVertex3f(b->right,
                       b->top,
                       b->front);
            glVertex3f(b->right,
                       TRACK_BASE,
                       b->front);
            glVertex3f(b->left,
                       TRACK_BASE,
                       b->front);

            /* left face */
            glNormal3f(-1, 0, 0);
            glVertex3f(b->left,
                       b->top,
                       b->front);
            glVertex3f(b->left,
                       TRACK_BASE,
                       b->front);
            glVertex3f(b->left,
                       TRACK_BASE,
                       b->back);
            glVertex3f(b->left,
                       b->top,
                       b->back);

            /* right face */
            glNormal3f(1, 0, 0);
            glVertex3f(b->right,
                       b->top,
                       b->front);
            glVertex3f(b->right,
                       b->top,
                       b->back);
            glVertex3f(b->right,
                       TRACK_BASE,
                       b->back);
            glVertex3f(b->right,
                       TRACK_BASE,
                       b->front);

            /* top face */
            glNormal3f(0, 1, 0);
            glVertex3f(b->left,
                       b->top,
                       b->front);
            glVertex3f(b->left,
                       b->top,
                       b->back);
            glVertex3f(b->right,
                       b->top,
                       b->back);
            glVertex3f(b->right,
                       b->top,
                       b->front);

            glEnd();
            
        }
    }

}

void track_destroy()
{
    GLushort i;
    for(i=0; i<track.num_positions; i++)
        free(track.track_rows[i]);
    free(track.track_rows);
    free_extra_colors();
    memset(track.bg_filename, 0, 256);
}

void track_list_read()
{
    char line[80];
    char tracklist_file[256];
    FILE *fp;
    int i,j;

    prepend_path(settings.track_path, DEFAULT_TRACKLIST, tracklist_file);

    fp = fopen(tracklist_file, "r");

    if(!fp)
    {
        fprintf(stderr,"Could not open track list file: %s\n", 
                tracklist_file);
        quit(-5);
    }

    /* line count */
    game_state.track_count = 0;
    while(fgets(line, 80, fp))
        game_state.track_count++;
    rewind(fp);

    /* allocate the track list */
    game_state.track_list = 
        (char**)malloc(game_state.track_count*sizeof(char*));

    for(i=0; i<game_state.track_count; i++)
        game_state.track_list[i] = (char*)calloc(80,sizeof(char));

    /* read in the names from the file */
    i = 0;
    while(fgets(line, 80, fp))
    {
        prepend_path(settings.track_path, line, game_state.track_list[i]);
        for(j=0; j<80; j++)
        {
            /* CHOMP */
            if(game_state.track_list[i][j] == '\n')
                game_state.track_list[i][j] = '\0';
        }
        i++;
    }

    fclose(fp);
}


void track_list_release()
{
    int i;

    for(i=0; i<game_state.track_count; i++)
        free(game_state.track_list[i]);
    free(game_state.track_list);

    game_state.track_count = 0;
}

static GLfloat * get_color(int i)
{
    if(i < STATIC_COLORS)
        return color_table[i];
    else
        return &extra_colors[i-STATIC_COLORS][0];
}

static void allocate_extra_colors()
{
    int i;

    extra_colors = (GLfloat**)malloc(track.high_extra_color*sizeof(GLfloat*));

    for(i=0; i<track.high_extra_color; i++)
    {
        extra_colors[i] = (GLfloat*)malloc(4*sizeof(GLfloat));

        /* entries will be black until they are assigned */
        extra_colors[i][0] = 0;
        extra_colors[i][1] = 0;
        extra_colors[i][2] = 0;
        extra_colors[i][3] = 1;
    }
}

static void free_extra_colors()
{
    int i;

    for(i=0; i<track.high_extra_color; i++)
        free(extra_colors[i]);
    free(extra_colors);
}

/* the functions below this line provide useful information about the
 * track to the collision detection routines 
 */

GLfloat track_height_at_point(vec3_t point)
{
    GLfloat i,j;
    
    i = track_i(point);
    j = track_j(point);

    if(j>=0 && j<NUM_LANES && i<track.num_positions)
        return track.track_rows[(GLshort)i][(GLshort)j].top;
    else
        return -50.0f;
}

GLshort track_j(vec3_t point)
{
    GLfloat j = (point[0] - NUM_LANES*LANE_WIDTH*-0.5) / LANE_WIDTH;
    return j;
}

GLshort track_i(vec3_t point)
{
    GLfloat i = point[2] / POSITION_LENGTH;
    return i;
}

trackdata_t* track_element_at_point(vec3_t point)
{
    return 
        (&track.track_rows[(GLushort)track_i(point)]
                          [(GLushort)track_j(point)]);
}

GLboolean track_height_increase(vec3_t a, vec3_t b)
{
    return (track_height_difference(a,b) < 0);
}

GLfloat track_to_point_dist(vec3_t p)
{
    return(p[1] - track_height_at_point(p));
}

GLboolean point_off_ground(vec3_t p)
{
    vec3_t x;
    VectorCopy(p,x);
    return (track_to_point_dist(x) > TRACK_BLOCK_BASE);
}

GLfloat track_to_ship(vec3_t p)
{
    GLfloat height = -9999;
    vec3_t x;

    VectorCopy(p, x);

    x[0] += SHIP_RIGHT;
    x[2] += SHIP_FRONT;
    height = MAX(height, track_height_at_point(x));
    x[0] -= (SHIP_RIGHT + SHIP_LEFT);
    height = MAX(height, track_height_at_point(x));
    x[2] -= (SHIP_FRONT + SHIP_BACK);
    height = MAX(height, track_height_at_point(x));
    x[0] += (SHIP_RIGHT + SHIP_LEFT);
    height = MAX(height, track_height_at_point(x));


    return height;
}
/* $Id$ */
