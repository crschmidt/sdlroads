/******************************************************************************
 *
 *  consts.h -- non-configurable constant values
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

/**************************************************************
 *
 * path constants (please end these with a '/')
 *
 **************************************************************/
#ifdef WIN32
#define CONFIG_DIR "./"
#define CONFIG_FILE "sdlroads.cfg"
#define PATH_PREFIX "./"
#else
#define CONFIG_DIR getenv("HOME")
#define CONFIG_FILE ".sdlroadsrc"
#endif
#define TRACK_DIR "tracks/"
#define SOUND_DIR "sound/"
#define IMAGE_DIR "images/"
#define MODEL_DIR "models/"
#define FONT_DIR "fonts/"
#define MAX_FILE_LENGTH 256

/**************************************************************
 *
 * menu constants
 *
 **************************************************************/
#define MENU_LEFT_OFFSET 50
#define MENU_SPACING 40

/**************************************************************
 *
 * track dimension constants
 *
 **************************************************************/
#define POSITION_LENGTH 1.3f
#define LANE_WIDTH 1.1f
#define NUM_LANES 7
#define TRACK_BASE 0.1f
#define TRACK_BLOCK_BASE 0.20f
#define TRACK_BLOCK_HEIGHT 0.6f
#define TRACK_RENDER_DIST 30

/**************************************************************
 *
 * Ship Constants
 *
 **************************************************************/
#define SHIP_ACCEL 0.002f
#define SHIP_GRAV -0.012f
#define SHIP_VEL_MAX 0.3f
#define SHIP_VEL_MIN 0.0f
#define SHIP_CROSS_VEL 0.05f
#define SHIP_JUMP_V0 0.18f
#define SHIP_BOUNCE -0.4f
#define SHIP_JUMP_CLOSE_ENOUGH .004f
#define SHIP_DEATH_HEIGHT -20.0f
#define SHIP_GRIND_DECEL 0.80f;
#define NUDGE 0.02f /* when floats let you down */

/**************************************************************
 *
 * Ship model size estimates:
 *
 **************************************************************/
#define MODEL_LEFT 1.63f
#define MODEL_RIGHT 1.63f
#define MODEL_TOP 0.97f
#define MODEL_BOTTOM 0.37f
#define MODEL_FRONT 2.79f
#define MODEL_BACK 0.74f

/**************************************************************
 *
 * Ship model actual sizes:
 *
 **************************************************************/
#define SHIP_LEFT 0.29f
#define SHIP_RIGHT 0.29f
#define SHIP_TOP 0.34f
#define SHIP_BOTTOM 0.0f
#define SHIP_FRONT 0.5f
#define SHIP_BACK 0.2f


/**************************************************************
 *
 * Camera Constants
 *
 **************************************************************/
#define CAMERA_X   0.0f 
#define CAMERA_Y   3.0f
#define CAMERA_LOOK_X 0.0f
#define CAMERA_LOOK_Y -0.55f
#define CAMERA_LOOK_Z 1.0f
#define CAMERA_Z_OFFSET -3.0f


/**************************************************************
 *
 * HUD Constants
 *
 **************************************************************/
#define HUD_BAR_WIDTH   0.03f 
#define HUD_EDGE_FACTOR 1.0f/1.05f
#define HUD_INV_EDGE_FACTOR 1.05f

/**************************************************************
 *
 * Hard-coded defaults for game settings
 *
 **************************************************************/
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_MUSICFILENAME "music.xm"
#define DEFAULT_FULLSCREEN 0
#define DEFAULT_GAMESTATE PLAY
#define DEFAULT_MODELNAME "rapier"
#define DEFAULT_TRACKLIST "tracks.list"
#define DEFAULT_SOUND_TOGGLE 0
#define DEFAULT_MUSIC_TOGGLE 0

/**************************************************************
 *
 * Draw settings
 *
 **************************************************************/
#define WINDOW_FPS 1000.0f/45.0f

/* $Id$ */
