/******************************************************************************
 *
 *  menu.c -- routines which present a menu
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

#include <stdio.h>
#include "menu.h"

static menu_t root, options, level, *current_menu;

/* a special type of action for when a menu is selected */
static void process();

static void increment_selection(menu_t *menu, GLshort delta);
static void free_menu(menu_t* menu);

/* menu-building helpers - each creates a type of menu entry */
static void create_menu(menu_t * menu, char title[32], 
        menu_t * parent, int itemcount);
static void create_zap(menu_t * parent, int position, 
        char title[32], menu_t * target);
static void create_level(menu_t * parent, int position, 
        char title[32], int level_number);
static void create_yes_no_option(menu_t * parent, int position, char title[32],
        void (*option_change_callback)(int selected));
static void render_option_list(int x_start, int y_start, option_data_t * odp);

void menu_init()
{
    option_data_t *odp;
    int i;
    char buf[32];

    /* root menu 
     */
    create_menu(&root, "SDLRoads", NULL, 3);

    create_zap(&root, 0, "Play", &level);
    create_zap(&root, 1, "Options", &options);
    create_zap(&root, 2, "Exit", NULL);

    root.selected = 0;

    /* level menu
     */
    create_menu(&level, "Tracks", &root, game_state.track_count + 1);

    /* make entries for the levels */
    for(i=0; i<level.itemcount; i++)
    {
        sprintf(buf, "track%d", i);
        create_level(&level, i, buf, i);
    }
    create_zap(&level, level.itemcount-1, "Exit", &root);

    /* options menu 
     */
    create_menu(&options, "Options", &root, 5);

    create_yes_no_option(&options, 0, "Fullscreen", &toggle_fullscreen);

    /* RESOLUTION - I have yet to think of a nicer way to populate this
     * evil struct */
    options.items[1].type = IT_OPTION;
    strcpy(options.items[1].text, "Resolution");
    options.items[1].itemdata = 
        (option_data_t*)malloc(sizeof(option_data_t));
    odp = (option_data_t*)options.items[1].itemdata;
    odp->optioncount = 4;
    odp->selected_option = width_res_code(settings.width);
    odp->option_headings = (char**)malloc(4*sizeof(char*));
    odp->option_headings[0] = (char*)malloc(16*sizeof(char));
    odp->option_headings[1] = (char*)malloc(16*sizeof(char));
    odp->option_headings[2] = (char*)malloc(16*sizeof(char));
    odp->option_headings[3] = (char*)malloc(16*sizeof(char));
    strcpy(odp->option_headings[0], "1280x1024");
    strcpy(odp->option_headings[1], "1024x768");
    strcpy(odp->option_headings[2], "800x600");
    strcpy(odp->option_headings[3], "640x480");
    odp->option_change_callback = &toggle_resolution;

	create_yes_no_option(&options, 2, "Sound Effects", &sound_toggle);
	create_yes_no_option(&options, 3, "Music", &music_toggle);

    /* EXIT */
    create_zap(&options, 4, "Exit", &root);

    options.selected = 0;

    current_menu = &root;
}

void menu_render()
{
    GLushort i,y_pos = 75, x_pos = 50;
    GLushort end_x;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0,0,1);
    print_large(current_menu->title, MENU_LEFT_OFFSET, y_pos);

    y_pos += 30;
    
    for(i=0; i<current_menu->itemcount; i++)
    {
        y_pos += MENU_SPACING;

        if(i == current_menu->selected)
            glColor3f(1,1,0);
        else
            glColor3f(1,1,1);

        if (y_pos > settings.height - 2*MENU_SPACING) 
        { 
            y_pos = 75 + 30 + MENU_SPACING; 
            x_pos += 200; 
        }

        print_small_s(current_menu->items[i].text, x_pos, y_pos, 
                &end_x, NULL);

        if(current_menu->items[i].type == IT_OPTION)
            render_option_list(end_x, y_pos, 
                    (option_data_t*)current_menu->items[i].itemdata);
    }
}

void menu_input(SDL_keysym *key)
{
    option_data_t *odp = NULL;

    switch(key->sym)
    {
        case SDLK_UP:
            increment_selection(current_menu, -1);
            break;

        case SDLK_DOWN:
            increment_selection(current_menu, 1);
            break;

        case SDLK_RIGHT:
            if(current_menu->items[current_menu->selected].type
                    == IT_OPTION)
            {
                odp = (option_data_t*)current_menu->items[current_menu->selected].itemdata;
                odp->selected_option++;
                CLAMP(odp->selected_option, 0, odp->optioncount-1);
				odp = (option_data_t*)current_menu->items[current_menu->selected].itemdata;
				(*(odp->option_change_callback))(odp->selected_option);
            }
            break;
        case SDLK_LEFT:
            if(current_menu->items[current_menu->selected].type
                    == IT_OPTION)
            {
                odp = (option_data_t*)current_menu->items[current_menu->selected].itemdata;
                odp->selected_option--;
                CLAMP(odp->selected_option, 0, odp->optioncount-1);
				odp = (option_data_t*)current_menu->items[current_menu->selected].itemdata;
				(*(odp->option_change_callback))(odp->selected_option);
            }
            break;

        case SDLK_RETURN:
        case SDLK_SPACE:
            process(current_menu);
            break;

        case SDLK_ESCAPE:
            /* make sure a level is loaded before jumping into play */
            if(game_state.level >= 0)
                game_state.state = PLAY;
            break;

        default:
            break;
    }
}

static void increment_selection(menu_t *menu, GLshort delta)
{
    menu->selected += delta;
    menu->selected = menu->selected % menu->itemcount;
    if(menu->selected < 0)
        menu->selected += menu->itemcount;
}

/* if you add an option, give it a line in here so that it is up-to-
 * date whenever the user enters this menu */
static void update_options()
{
    ((option_data_t*)options.items[1].itemdata)->selected_option = 
        width_res_code(settings.width);
    ((option_data_t*)options.items[0].itemdata)->selected_option =
        settings.fullscreen;
    ((option_data_t*)options.items[2].itemdata)->selected_option =
        settings.sound_on;
    ((option_data_t*)options.items[3].itemdata)->selected_option =
        settings.music_on;
}

static void process()
{
    /* screw memory efficiency.  this saves me typing */
    menuitem_t* selected = &current_menu->items[current_menu->selected];

    switch(selected->type)
    {
        case IT_LEVEL:
            /* FIXME: this isn't going to always be true... */
            game_state.state = PLAY;
			set_level(
                *(int*)current_menu->items[current_menu->selected].itemdata);
            scene_init();
            break;

        case IT_ZAP:
            if((menu_t*)(selected->itemdata) == NULL)
                quit(0);

            if((menu_t*)(selected->itemdata) == &options)
                update_options();

            /* the user tends not to like to come back to a menu with
             * `Exit' selected */
            if(&(current_menu->items[current_menu->itemcount-1]) == selected)
                current_menu->selected = 0;

            current_menu = (menu_t*)(selected->itemdata);
            break;

        case IT_OPTION:
            /* TODO: this one is going to be complicated *
            odp = (option_data_t*)current_menu->items[current_menu->selected].itemdata;
            (*(odp->option_change_callback))(odp->selected_option);
			*/
            break;

        default:
            break;
    }
}

void menu_destroy()
{
    /*
    free_menu(&root);
    free_menu(&options);
    free_menu(&level);
    */
}

void free_menu(menu_t *menu)
{
    option_data_t *odp = NULL;
    int j,i;

    for(i=0; i<menu->itemcount; i++)
    {
        if(menu->items[i].type == IT_OPTION)
        {
            odp = (option_data_t*)menu->items[i].itemdata;
            for(j=0; j<odp->optioncount; j++)
                free(odp->option_headings[j]);
            free(odp->option_headings);
        }
        free(menu->items[i].itemdata);
    }
    free(menu->items);
}


static void create_menu(menu_t * menu, char title[32], 
        menu_t * parent, int itemcount)
{
    menu->itemcount = itemcount;
    menu->items = (menuitem_t*)malloc(menu->itemcount*sizeof(menuitem_t));
    menu->parent = parent;
    strcpy(menu->title, title);
}


static void create_zap(menu_t * parent, int position, 
        char title[32], menu_t * target)
{
    parent->items[position].type = IT_ZAP;
    parent->items[position].itemdata = target;
    strcpy(parent->items[position].text, title);
}

static void create_level(menu_t * parent, int position, 
        char title[32], int level_number)
{
    parent->items[position].type = IT_LEVEL;
    parent->items[position].itemdata = (int*)malloc(sizeof(int));
    *(int*)parent->items[position].itemdata = level_number;
    strcpy(parent->items[position].text, title);
}


static void render_option_list(int x_start, int y_start, option_data_t * odp)
{
    int i;

    for(i=0; i<odp->optioncount; i++)
    {
        x_start += 15;

        if(i == odp->selected_option)
            glColor3f(0,1,0);
        else
            glColor3f(1,1,1);

        print_small_s(odp->option_headings[i], x_start, y_start,
                &x_start, NULL);
    }
}

static void create_yes_no_option(menu_t * parent, int position, char title[32],
        void (*option_change_callback)(int selected))
{
    option_data_t * odp;

    parent->items[position].type = IT_OPTION;
    strcpy(parent->items[position].text, title);
    parent->items[position].itemdata = 
        (option_data_t*)malloc(sizeof(option_data_t));
    odp = (option_data_t*)parent->items[position].itemdata;
    odp->optioncount = 2;
    odp->selected_option = settings.fullscreen;
    odp->option_headings = (char**)malloc(2*sizeof(char*));
    odp->option_headings[0] = (char*)malloc(16*sizeof(char));
    odp->option_headings[1] = (char*)malloc(16*sizeof(char));
    strcpy(odp->option_headings[0], "No");
    strcpy(odp->option_headings[1], "Yes");
    odp->option_change_callback = option_change_callback;
}
/* $Id$ */
