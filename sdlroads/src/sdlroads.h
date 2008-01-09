#ifndef __SDLROADS_H__
#define __SDLROADS_H__

#ifdef WIN32
# include "win32config.h"
#elif defined(HAVE_CONFIG_H)
# include "config.h"
# define SDLROADS_VERSION VERSION
# ifndef HAVE_LIBSDL_MIXER
#  define NO_SOUND
# endif
#else
# error "this needs to be configured somehow"
#endif

#include "consts.h"
#include "defs.h"
#include "mathlib.h"

///////////////////////////////////////////////////////////////////////
// Settings and Config
extern int parse_args(int argc, char** argv, game_options_t *config);
extern int settings_init(int argc, char **argv);
extern int settings_file_write();

///////////////////////////////////////////////////////////////////////
// Sounds
extern void sound_init();
extern void crash_sound();
extern void sound_kill();
extern void music_toggle(int onoff);
extern void sound_toggle(int onoff);

///////////////////////////////////////////////////////////////////////
// Camera Control
extern void camera_spin(float angle);
extern void camera_tilt(float angle);
extern void camera_update();
extern void camera_init();

///////////////////////////////////////////////////////////////////////
// Menu
extern void menu_init();
extern void menu_render();
extern void menu_input(SDL_keysym *key);
extern void menu_destroy();

///////////////////////////////////////////////////////////////////////
// Scene
extern void bg_init(char *filename);

extern void scene_update();
extern void scene_init();
extern void set_level(int l);

///////////////////////////////////////////////////////////////////////
// Renderer
extern void render_init();
extern void render_draw();
extern void render_kill();

///////////////////////////////////////////////////////////////////////
// Models
extern GLuint mesh_cache(char* name, mesh_t* mesh);
extern void destroy_mesh(mesh_t* mesh);
extern void render_exploding(mesh_t* mesh);

///////////////////////////////////////////////////////////////////////
// Input
void input_keyup( SDL_keysym* key );
void input_keydown( SDL_keysym* key );

///////////////////////////////////////////////////////////////////////
// The Ship
extern void ship_init(ship_t* s);
extern void ship_render(ship_t* s);
extern void ship_update(ship_t* s);
extern void ship_jump(ship_t* s);
extern void ship_die(ship_t* s, int cause);
extern void ship_restart(ship_t* s);
extern void ship_win(ship_t* s);
extern void ship_uninitialize(ship_t *s);
extern void ship_sides(ship_t* s);

///////////////////////////////////////////////////////////////////////
// Text
extern void text_init();
extern void text_kill();
extern void print_large(const char* string, Uint16 start_x, Uint16 start_y);
extern void print_small(const char* string, Uint16 start_x, Uint16 start_y);
extern void print_large_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y);
extern void print_small_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y);

///////////////////////////////////////////////////////////////////////
// Level Tracks
extern GLboolean track_load();
extern void track_render(GLushort start, GLushort stop);
extern void track_destroy();
extern GLfloat track_height_at_point(vec3_t point);
extern GLshort track_i(vec3_t point);
extern GLshort track_j(vec3_t point);
extern trackdata_t* track_element_at_point(vec3_t point);
extern GLboolean track_height_increase(vec3_t a, vec3_t b);
extern GLfloat track_to_point_dist(vec3_t p);
extern GLboolean point_off_ground(vec3_t p);
extern GLfloat track_to_ship(vec3_t p);
extern void track_list_read();
extern void track_list_release();

///////////////////////////////////////////////////////////////////////
// Window
extern int window_init();
extern void window_draw();
extern void window_restart();

///////////////////////////////////////////////////////////////////////
// Miscellaneous Utilities
extern void prepend_path(const char* dir, const char* file, char* s);
extern void prepend_config_path(const char *file, char *s);
extern void toggle_fullscreen(int on_off);
extern int res_code_width(int res_code);
extern int res_code_height(int res_code);
extern int width_res_code(int width);
extern void toggle_resolution(int res_code);
extern void error_print(const char * where, const char * str);

#endif