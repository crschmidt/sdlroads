/******************************************************************************
 *
 *  render.c -- where all the drawing originates
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
#include "camera.h"
#include "ship.h"
#include "images.h"
#include "text.h"
#include "args.h"

GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat mat_white[] = {0.9, 0.9, 0.9, 1.0};
GLfloat light_pos[] = {0, 5, 3, 0};
GLuint enter2d, enter3d, exit2d, drawbg; 

static void fg_render();

void render_init()
{
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    enter2d = glGenLists(4);
    enter3d = enter2d+1;
    exit2d = enter2d+2;
    drawbg = enter2d+3;

    glNewList(enter3d, GL_COMPILE_AND_EXECUTE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0,0, settings.width, settings.height);
        gluPerspective(80.0f, settings.aspect_ratio, 0.1f, 
                       POSITION_LENGTH*(TRACK_RENDER_DIST-2) );
    glEndList();

    glNewList(enter2d, GL_COMPILE);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glViewport(0,0, settings.width, settings.height);
        gluOrtho2D( -1.0, 1.0, -1.0, 1.0 );

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(0, 0, 1,
                0, 0, 0,
                0, 1, 0);
    glEndList();

    glNewList(exit2d, GL_COMPILE);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glEndList();

    glNewList(drawbg, GL_COMPILE);
        glCallList(enter2d);

        glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT);
        glColor3i(0,0,0);

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, game_state.bg_tex_num);


        glBegin(GL_QUADS);
        glColor3f(0.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0);
        glEnd();

        glCallList(exit2d);

        glPopAttrib();
    glEndList();


    glShadeModel(GL_SMOOTH);

    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_white);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mat_ambient);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);

    glColor3f(1.0, 0.5, 0.5);

}

void render_kill()
{
    glDisable(GL_LIGHT0);
    glDeleteLists(enter2d, 4);
}

void render_draw()
{
    static int frames = 0;
    static int time_base;
    static char fps_string[80];
    GLshort start;
    int elapsed = SDL_GetTicks()-time_base;
    
    if(elapsed >= 5000)
    {
        int time = SDL_GetTicks() - time_base;
        sprintf(fps_string, "%d frames in 5.0 seconds = %0.3f FPS", 
                frames, 1000.0*frames/elapsed);
        puts(fps_string);
        frames = 0;
        time_base = SDL_GetTicks();
    }
    frames++;

    /* Basic Scene Initialization */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            camera_pos[0],
            camera_pos[1],
            camera_pos[2],
            camera_pos[0] + camera_look[0],
            camera_pos[1] + camera_look[1],
            camera_pos[2] + camera_look[2],
            camera_up[0],
            camera_up[1],
            camera_up[2]);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glCallList(drawbg);

    start = track_i(player.pos) - 3;

    CLAMP(start, 0, track.num_positions);

    track_render(start, start+TRACK_RENDER_DIST);
    
    ship_render(&player);

    fg_render();

    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    print_small(fps_string, settings.width * .025, settings.height * .05);

    if(game_state.victory)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        print_small("Course Completed.", settings.width / 2 - 60, settings.height / 2 - 40);
    }

    if(game_state.paused)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        print_small("Game Paused.\n'P' to unpause.", settings.width / 2 - 45, settings.height / 2 - 20);
    }
}


static void fg_render()
{
    float tint = player.pos[1] / (SHIP_DEATH_HEIGHT - TRACK_BLOCK_BASE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    /* Viewmodel changing code */
    glCallList(enter2d);

    glScalef(HUD_EDGE_FACTOR, HUD_EDGE_FACTOR, 1.0);

    glBegin(GL_QUADS);
    
    glColor4f(0.8, 0.2, 0.2,0.5);
    glVertex2f(-1.0, 1.0);
    glVertex2f((player.pos[2]/track.num_positions/POSITION_LENGTH*2 - 1) , 1.0);
    glVertex2f((player.pos[2]/track.num_positions/POSITION_LENGTH*2 - 1) , 1.0 - HUD_BAR_WIDTH);
    glVertex2f(-1.0, 1.0 - HUD_BAR_WIDTH);
    
    glColor4f(0.2, 0.8, 0.2,0.8);
    glVertex2f(-1.0, -1.0);
    glVertex2f((player.vel[2]/SHIP_VEL_MAX*2 - 1) , -1.0);
    glVertex2f((player.vel[2]/SHIP_VEL_MAX*2 - 1) , -1.0 + HUD_BAR_WIDTH);
    glVertex2f(-1.0, -1.0 + HUD_BAR_WIDTH);

    
    /* if I knew what I was doing, there wouldn't be 2's below */
    if(tint > 0)
    {
        glColor4f(0.0, 0.0, 0.0, tint);
        glVertex2f(-2.0, -2.0);
        glVertex2f(2.0,  -2.0);
        glVertex2f(2.0,   2.0);
        glVertex2f(-2.0,  2.0);
    }

    if(game_state.paused)
    {   
        glColor4f(0.0, 0.0, 0.0, 0.5);
        glVertex2f(-2.0, -2.0);
        glVertex2f(2.0,  -2.0);
        glVertex2f(2.0,   2.0);
        glVertex2f(-2.0,  2.0);
    }
    glEnd();
    
    /* some nice crosshairs in case i need to center stuff 
    glBegin(GL_LINES);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2f(0.0, 0.2);
    glVertex2f(0.0, -0.2);
    glVertex2f(-0.2, 0.0);
    glVertex2f(0.2, 0.0);
    glEnd(); */

    glCallList(exit2d);

    glPopAttrib();
}

/* $Id: render.c,v 1.26.2.1 2005/01/23 23:11:09 y3t39 Exp $ */
