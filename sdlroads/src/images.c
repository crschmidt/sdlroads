/******************************************************************************
 *
 *  images.c -- image loading functions
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

#include "SDL_image.h"
#include <stdio.h>


static int image_read_png(const char* filename, GLubyte** image_data, 
        GLushort* height, GLushort* width);
static int image_read(const char* filename);

void bg_init(char *filename)
{
    int i = 0;

    while(filename[i] != '\0') 
        i++;
    filename[i-1] = '\0';
    
    game_state.bg_tex_num = image_read(filename);
}


/* read in an image, and return the texture number */
static int image_read(const char* filename)
{
    SDL_PixelFormat* pf;
    SDL_Surface *img_surface;
    GLuint texnum;
    GLenum e;
    
    img_surface = IMG_Load(filename);
    pf = img_surface->format;
    
    glGenTextures(1, &texnum);
    glBindTexture(GL_TEXTURE_2D, texnum);

    if(pf->BitsPerPixel > 8)
    {
        e = (pf->BitsPerPixel >= 32) ? GL_RGBA : GL_RGB;

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 
                0, 
                pf->BytesPerPixel, 
                img_surface->w, 
                img_surface->h, 
                0, 
                e, 
                GL_UNSIGNED_BYTE, 
                img_surface->pixels);
    }
    else
    {
        /* TODO: interpret pallete */
    }

    SDL_FreeSurface(img_surface);

    return texnum;
}

#ifdef HAVE_LIBPNG
#include <png.h>

#define SIGBYTES 8

static int image_read_png(const char* filename, GLubyte** image_data, 
        GLushort* height, GLushort* width )
{
    FILE* fp = fopen(filename, "rb");

    char head[SIGBYTES];
    GLubyte* cur_ptr;
    int i;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytepp row_pointers;
    png_uint_32 row_length;

    if(!fp) 
        return 0;
    

    fread(head, 1, SIGBYTES, fp);

    if(png_sig_cmp(head,0,SIGBYTES) != 0)
    {
        fprintf(stderr, "File \"%s\" is not a PNG.\n", filename);
        fclose(fp);
        return 0;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
            (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    if(!png_ptr)
    {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fputs("Info structure read error", stderr);
        fclose(fp);
        return 0;
    }

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_set_sig_bytes(png_ptr, SIGBYTES);
    png_init_io(png_ptr, fp);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16|
                                    PNG_TRANSFORM_EXPAND|
                                    PNG_TRANSFORM_PACKING, NULL);

    row_pointers = png_get_rows(png_ptr, info_ptr);
    row_length = png_get_rowbytes(png_ptr, info_ptr);

    *height = png_get_image_height(png_ptr, info_ptr);
    *width = png_get_image_width(png_ptr, info_ptr);

    *image_data=(GLubyte*)malloc(*height * row_length*sizeof(GLubyte));
    cur_ptr = *image_data;

    /* don't ask me why images load backwards... */
    for(i=*height - 1; i>=0; i--)
    {
        memcpy(cur_ptr, row_pointers[i], row_length);
        cur_ptr += row_length;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 1;
}
#endif
/* $Id: images.c,v 1.7 2004/12/31 00:31:23 y3t39 Exp $ */
