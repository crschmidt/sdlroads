/******************************************************************************
 *
 *  text.c -- for outputting text to the window
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

#include "defs.h"
#include "text.h"
#include "consts.h"

typedef struct
{
    int minx, maxx,
        miny, maxy,
        advance;
} glyph_metric_t;

static TTF_Font *font_small, *font_large;

static SDL_Color black = {0, 0, 0};
static SDL_Color white = {255,255,255};

static SDL_Surface *letters[128];
static glyph_metric_t gm_small[128], gm_large[128];
static GLuint small_letters[128];
static GLuint big_letters[128];

static GLuint start2Dtext, end2Dtext;

static void inline text_putchar(GLuint* font_set, glyph_metric_t* gm, 
                                char letter, int x, int y);
static void inline text_putstring(GLuint* font_set, glyph_metric_t* gm, 
        TTF_Font* font, const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y);

static void build_letter_textures(TTF_Font* font, GLuint* letter_textures, 
                                  glyph_metric_t* gm);
static GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord);

/*************************************************
 *
 * Font initialization
 *
 ************************************************/
void text_init()
{
    char buf[256];

    if(TTF_Init() < 0)
        fprintf(stderr, "TTF initialization failure!\n");

    prepend_path(FONT_DIR, "drg.ttf", buf);
    font_large = TTF_OpenFont(buf, 60);

    prepend_path(FONT_DIR, "ganymede.ttf", buf);
    font_small = TTF_OpenFont(buf, 14);

    if(font_small == NULL)
        fprintf(stderr, "font_small is NULL: %s\n", TTF_GetError());

    build_letter_textures(font_small, small_letters, gm_small);
    build_letter_textures(font_large, big_letters, gm_large);

    start2Dtext = glGenLists(2);
    end2Dtext = start2Dtext + 1;

    glNewList(start2Dtext, GL_COMPILE);
        glPushAttrib(GL_ENABLE_BIT);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glViewport(0, 0, settings.width, settings.height);
        gluOrtho2D(0, settings.width, 0, settings.height);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(0, 0, 1,
                  0, 0, 0,
                  0, 1, 0);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEndList();

    glNewList(end2Dtext, GL_COMPILE);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glPopAttrib();
    glEndList();
}

/* Quick utility function for texture creation */
static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

/*
 * this function stolen from glfont.c example in SDL_ttf
 */
GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	/* Use the surface width and height expanded to powers of 2 */
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = 0.0f;			/* Min X */
	texcoord[1] = 0.0f;			/* Min Y */
	texcoord[2] = (GLfloat)surface->w / w;	/* Max X */
	texcoord[3] = (GLfloat)surface->h / h;	/* Max Y */

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	/* Save the alpha blending attributes */
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	/* Create an OpenGL texture for the image */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     w, h,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);
	SDL_FreeSurface(image); /* No longer needed */

	return texture;
}


static void build_letter_textures(TTF_Font* font, GLuint* letter_textures, 
                                  glyph_metric_t* gm)
{
    Uint16 i, x_fix, y_fix, tex_w, tex_h;
    SDL_Surface* new_letter;
    GLfloat unused[4];
    GLenum gl_error;

    for(i=0; i<128; i++)
    {
        TTF_GlyphMetrics(font, i, 
                &gm[i].minx, &gm[i].maxx,
                &gm[i].miny, &gm[i].maxy,
                &gm[i].advance);

        new_letter = TTF_RenderGlyph_Blended(font, i, white);

        tex_w = power_of_two(new_letter->w);
        tex_h = power_of_two(new_letter->h);

        x_fix = tex_w - (gm[i].maxx - gm[i].minx);
        y_fix = tex_h - (gm[i].maxy - gm[i].miny);

        gm[i].maxx += x_fix;
        gm[i].miny -= y_fix;

        if(new_letter)
        {
            glGetError();
            letter_textures[i] = SDL_GL_LoadTexture(new_letter, unused);
            if ( (gl_error = glGetError()) != GL_NO_ERROR ) {
                /* If this failed, the text may exceed texture size limits */
                printf("Warning: Couldn't create texture: 0x%x\n", gl_error);
            }
            SDL_FreeSurface(new_letter);
        }
        else
            fprintf(stderr, "Error rendering char %hu\n", i);
    }
}


/************************************************
 *
 * Font rendering
 *
 ************************************************/
static void inline text_putchar(GLuint* font_set, glyph_metric_t* gm, 
                         char letter, int x, int y)
{

    glBindTexture(GL_TEXTURE_2D, font_set[letter]);
    glBegin(GL_QUADS);

    glTexCoord2f(0,1); glVertex2f(x+gm[letter].minx, y+gm[letter].miny);
    glTexCoord2f(1,1); glVertex2f(x+gm[letter].maxx, y+gm[letter].miny);
    glTexCoord2f(1,0); glVertex2f(x+gm[letter].maxx, y+gm[letter].maxy);
    glTexCoord2f(0,0); glVertex2f(x+gm[letter].minx, y+gm[letter].maxy); 

    glEnd();


}

static void inline text_putstring(GLuint* font_set, glyph_metric_t* gm, 
        TTF_Font* font, const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y)
{
    Uint16 i = 0; 
    Uint16 x_cursor = start_x,
           y_cursor = start_y;

    glCallList(start2Dtext);

    while(string[i] != '\0')
    {
        if(string[i] == '\n')
        {
            x_cursor = start_x;
            y_cursor -= TTF_FontLineSkip(font);
        }
        else
        {
            text_putchar(font_set, gm, string[i], x_cursor, y_cursor);
            x_cursor += gm[string[i]].advance;
        }
        i++;
    }

    glCallList(end2Dtext);

    /* the user passed us the upper left of where the string should
     * begin, now we return the bottom right */
    if(end_x) *end_x = x_cursor;
    if(end_y) *end_y = settings.height - (y_cursor - TTF_FontLineSkip(font));
}

void print_large(const char* string, Uint16 start_x, Uint16 start_y)
{
    text_putstring(big_letters, gm_large, font_large, string, start_x, 
            settings.height - start_y - TTF_FontAscent(font_large),
            NULL, NULL);
}

void print_small(const char* string, Uint16 start_x, Uint16 start_y)
{
    text_putstring(small_letters, gm_small, font_small, string, start_x, 
            settings.height - start_y - TTF_FontAscent(font_small),
            NULL, NULL);
}

void print_large_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y)
{
    text_putstring(big_letters, gm_large, font_large, string, start_x, 
            settings.height - start_y - TTF_FontAscent(font_large),
            end_x, end_y);
}

void print_small_s(const char* string, Uint16 start_x, Uint16 start_y,
        Uint16 * end_x, Uint16 * end_y)
{
    text_putstring(small_letters, gm_small, font_small, string, start_x, 
            settings.height - start_y - TTF_FontAscent(font_small),
            end_x, end_y);
}

/************************************************
 *
 * Cleanup functions
 *
 ************************************************/
void text_kill()
{
    if (font_large != NULL) {
        TTF_CloseFont(font_large);
    } 
    if (font_small != NULL) {
        TTF_CloseFont(font_small);
    }    
    TTF_Quit();
}
/* $Id: text.c,v 1.15.2.1 2005/01/23 23:11:09 y3t39 Exp $ */
