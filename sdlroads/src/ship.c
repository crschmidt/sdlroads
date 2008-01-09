/******************************************************************************
 *
 *  ship.c -- ship related implementation and collision detection
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

#include <math.h>

enum death_t {FALL, CRASH};


void ship_init(ship_t* s)
{
    /* sometimes the ship isn't at the beginning of a track */
    if(s->save == GL_FALSE)
    {
        s->pos[0] = 0;  
        s->pos[1] = 1; 
        s->pos[2] = 1;
        s->vel[1] = s->vel[2] = 0;

        if(s->acc[2] < SHIP_ACCEL) /* condition added by request */
            s->acc[2] = 0;

        s->on_ground = 0;
    }
    else
        s->save = GL_FALSE;


    s->exploding = 0;

    s->model.explosion_frame = 0;

    /* scale the model to the box size */
    s->z_scale = (SHIP_FRONT + SHIP_BACK)/(MODEL_FRONT + MODEL_BACK);
    s->x_scale = (SHIP_LEFT + SHIP_RIGHT)/(MODEL_LEFT + MODEL_RIGHT);
    s->y_scale = (SHIP_TOP + SHIP_BOTTOM)/(MODEL_TOP + MODEL_BOTTOM);
    s->z_shift = SHIP_FRONT - MODEL_FRONT*s->z_scale;
    s->y_shift = SHIP_TOP - MODEL_TOP*s->y_scale;
    s->x_shift = SHIP_RIGHT - MODEL_RIGHT*s->x_scale;

    if(!s->initialized)
    {
        s->display_list = mesh_cache("rapier", &s->model);
        s->initialized = GL_TRUE;
    }

    camera_init();
}


void ship_uninitialize(ship_t *s)
{
    s->initialized = 0;
    glDeleteLists(s->display_list, 1);
    destroy_mesh(&s->model);
}


void ship_render(ship_t* s)
{
    vec3_t p;
    VectorCopy(s->pos, p);

    glPushAttrib(GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT|GL_POLYGON_BIT);
    glPushMatrix();

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glTranslatef(s->pos[0], s->pos[1], s->pos[2]);

    glPushMatrix();
    glScalef(s->x_scale, s->y_scale, s->z_scale);
    glTranslatef(s->x_shift, s->y_shift, s->z_shift);


    if(!s->exploding)
        glCallList(s->display_list);
    else
        render_exploding(&s->model);


    glPopMatrix();
    glPopMatrix();

    glDisable(GL_LIGHTING);

    /* bounding box 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    glTranslatef(0, 0.01-SHIP_BOTTOM, 0);
    glColor3f(0,1,0);
    glBegin(GL_POLYGON);
        glVertex3fv(s->front_r);
        glVertex3fv(s->front_l);
        glVertex3fv(s->back_l);
        glVertex3fv(s->back_r);
    glEnd();
    glPopMatrix();
    */

    glPopAttrib();

}

void ship_update(ship_t* s)
{
    vec3_t x,y;

    if(s->on_ground)
        s->acc[1] = 0;
    else
        s->acc[1] = SHIP_GRAV;

    VectorAdd(s->vel, s->acc, s->vel);

	CLAMP(s->vel[2], SHIP_VEL_MIN, SHIP_VEL_MAX);

    if(s->bottom[1] > track_to_ship(s->bottom))
        s->on_ground = 0;

    if(!s->exploding)
        VectorAdd(s->vel, s->pos, s->pos);

    ship_sides(s);

    /***************************************************************
     *
     * Collision Detection
     *
     ***************************************************************/


    if(s->top[1] >= TRACK_BASE) /* no collisions under the track */
    {
        /* Head on collisions */
        VectorCopy(s->front_l, x);
        VectorCopy(s->front_r, y);
        x[2] -= s->vel[2];
        y[2] -= s->vel[2];
        if((track_height_increase(x, s->front_l) || track_height_increase(y, s->front_r)))
        {
            if(s->bottom[1]+NUDGE < MAX(track_height_at_point(s->front_r), track_height_at_point(s->front_l)))
            {
                /*printf("ship bottom: %f\ntrack_height_at s->front_r: %f\ntrack_height_at s->front_l: %f\nMAX: %f\n",
                        s->bottom[1], track_height_at_point(s->front_r), track_height_at_point(s->front_l),
                        MAX(track_height_at_point(s->front_r), track_height_at_point(s->front_l)));*/
                s->pos[2] -= s->vel[2];
                ship_sides(s);
                if (s->vel[2] > (SHIP_VEL_MAX*0.375f))  /* is 3/8 fair? */
                    ship_die(s, CRASH); 
                s->collision_speed = s->vel[2];
                s->vel[2] = 0;
            }
        }

        /* right side */
        VectorCopy(s->front_r, x);
        VectorCopy(s->back_r, y);
        x[0] -= s->vel[0];
        y[0] -= s->vel[0];
        if(track_height_increase(x, s->front_r) ||
           track_height_increase(y, s->back_r))
        {
            if(s->bottom[1]+NUDGE < MAX(track_height_at_point(s->front_r), 
                                        track_height_at_point(s->back_r)))
            {
                /* Side of ship hit wall... */
                s->pos[0] -= s->vel[0];
                ship_sides(s);
                s->vel[2] *= SHIP_GRIND_DECEL;
            }
        }

        /* left side */
        VectorCopy(s->front_l, x);
        VectorCopy(s->back_l, y);
        x[0] -= s->vel[0];
        y[0] -= s->vel[0];
        if(track_height_increase(x, s->front_l) ||
           track_height_increase(y, s->back_l))
        {
            if(s->bottom[1]+NUDGE < MAX(track_height_at_point(s->front_l), 
                                        track_height_at_point(s->back_l)))
            {
                s->pos[0] -= s->vel[0];
                ship_sides(s);
                s->vel[2] *= SHIP_GRIND_DECEL;
            }
        }


        /* Fall off of blocks moving forward */
        VectorCopy(s->back_r, x);
        VectorCopy(s->back_l, y);
        x[2] -= s->vel[2];
        y[2] -= s->vel[2];
        if(track_height_increase(s->back_r, x) || track_height_increase(s->back_l, y))
            s->on_ground = 0;

        /* Hit the ground */
        if(!(s->on_ground))
        {
            if(s->bottom[1] <= track_to_ship(s->bottom) && s->vel[1] < 0) 
            {
                s->pos[1] = SHIP_BOTTOM + track_to_ship(s->bottom); // potential landing weakness
                ship_sides(s);
                s->vel[1] *= SHIP_BOUNCE;
                if(s->vel[1] > 0 && s->vel[1] < 0.01f &&
                        (s->bottom[1] - track_height_at_point(s->pos)) < 0.0001)
                {
                    s->on_ground = 1;
                    s->vel[1] = 0;
                }
            }
        }

    }

    if (s->top[1] <= (SHIP_DEATH_HEIGHT)) 
        ship_die(s, FALL);

    if (s->front_r[2] >= 
            (track.num_positions*POSITION_LENGTH) && s->on_ground) 
        ship_win(s);
}

void ship_sides(ship_t* s)
{
    VectorCopy(s->pos, s->front_r);
    VectorCopy(s->pos, s->front_l);
    VectorCopy(s->pos, s->top);
    VectorCopy(s->pos, s->bottom);
    VectorCopy(s->pos, s->back_r);
    VectorCopy(s->pos, s->back_l);

    s->front_r[0] = s->pos[0] + SHIP_RIGHT;
    s->front_r[2] = s->pos[2] + SHIP_FRONT;

    s->front_l[0] = s->pos[0] - SHIP_LEFT;
    s->front_l[2] = s->pos[2] + SHIP_FRONT;

    s->top[1] = s->pos[1] + SHIP_TOP;
    s->bottom[1] = s->pos[1] - SHIP_BOTTOM;

    s->back_r[0] = s->pos[0] + SHIP_RIGHT;
    s->back_r[2] = s->pos[2] - SHIP_BACK;

    s->back_l[0] = s->pos[0] - SHIP_LEFT;
    s->back_l[2] = s->pos[2] - SHIP_BACK;
}

void ship_jump(ship_t* s)
{
    if(s->on_ground || 
       fabs(s->bottom[1] + SHIP_JUMP_CLOSE_ENOUGH/SHIP_GRAV) < 
                           track_height_at_point(s->pos) )
    {
        s->on_ground = 0;
        s->vel[1] = s->ship_jump_v0;
    }
}

void ship_die(ship_t* s, int cause) 
{   
    if(cause == FALL)
    {
        printf("You have fallen, restarting.\n");
        ship_restart(s); 
    }
    else
    {
        printf("You have crashed, restarting.\n");
        crash_sound();
        s->exploding = 1;
    }
}

void ship_restart(ship_t* s) {
    s->model.explosion_frame = 0.0f;
    ship_init(s);
}

void ship_win(ship_t* s) 
{
    printf("You win.\n");
    game_state.victory = SDL_GetTicks();
/*    game_state.paused = 1;
    scene_update();
    render_draw();
    sleep(1); 
    game_state.victory = 0;
    game_state.paused = 0; 
    game_state.level++;*/
}
/* $Id: ship.c,v 1.27 2005/01/12 23:45:13 y3t39 Exp $ */
