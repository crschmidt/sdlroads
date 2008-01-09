/******************************************************************************
 *
 *  model.c -- Model loading functions
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

#include <ctype.h>

static GLshort parse_mtl_file(char* name, material_t** materials);
static void parse_obj_file(char* name, mesh_t* mesh);
static GLint material_index(const char* name, mesh_t* m);
static GLuint generate_display_list(mesh_t* mesh);
static GLushort token_count(const char* string);
static void process_face_line(const char* line, GLuint fi, mesh_t* mesh);


/* this function encapsulates the process of loading a mesh model. */
GLuint mesh_cache(char* name, mesh_t* mesh)
{
    GLuint list;

    mesh->mat_count = parse_mtl_file(name, &mesh->materials);
    parse_obj_file(name, mesh);
    list = generate_display_list(mesh);

    puts("[model]: mesh loaded");

    return list;
}

static void parse_obj_file(char* name, mesh_t* mesh)
{
    FILE* fp;
    char buf[256];
    char filename[256];
    GLuint fi = 0;
    GLuint vi = 0;
    GLuint ni = 0;
    GLushort current_material = 0;
    GLint x;


    mesh->face_count = 0;
    mesh->vertex_count = 0;
    mesh->normal_count = 0;
    

    prepend_path(MODEL_DIR, name, filename);
    strcat(filename, ".obj");

    fp = fopen(filename, "r");

    while(fgets(buf, 256, fp))
    {
        if(isspace(buf[0]))
            continue;

        switch(buf[0])
        {
            case 'v':
                if(buf[1] == 'n')
                    mesh->normal_count++;
                else
                    mesh->vertex_count++;
                break;
            case 'f':
                mesh->face_count++;
                break;
        }
    }

    mesh->faces = (face_t*)malloc(mesh->face_count * sizeof(face_t));
    mesh->vertices = (vec3_t*)malloc(mesh->vertex_count * sizeof(vec3_t));
    mesh->normals = (vec3_t*)malloc(mesh->normal_count * sizeof(vec3_t));

    rewind(fp);

    while(fgets(buf, 256, fp))
    {
        if(isspace(buf[0]))
            continue;

        switch(buf[0])
        {
            case 'v':
                if(buf[1] == 'n')
                {
                    sscanf(buf, "vn %f %f %f",
                            &mesh->normals[ni][0],
                            &mesh->normals[ni][1],
                            &mesh->normals[ni][2]);
                    ni++;
                }
                else
                {
                    sscanf(buf, "v %f %f %f", 
                            &mesh->vertices[vi][0],
                            &mesh->vertices[vi][1],
                            &mesh->vertices[vi][2]);
                    vi++;
                }
                break;
            case 'f':
                process_face_line(buf, fi, mesh);
                mesh->faces[fi].mat_index = current_material;
                fi++;
                break;
            case 'u':
                x = material_index(&buf[7], mesh);
                if(x == -1)
                {
                    fprintf(stderr, "A material name was not found!\n%s\n",
                            buf);
                }
                else
                    current_material = x;

                break;
        }
    }

    fclose(fp);
}

static void process_face_line(const char* buf, GLuint fi, mesh_t* mesh)
{
    char** tokens;
    GLubyte i = 0;
    char* bufpos;
    char* line;

    line = (char*)_strdup(buf);

    mesh->faces[fi].vertex_count = token_count(&buf[2]);
    mesh->faces[fi].v_indices = 
        (GLuint*)malloc(mesh->faces[fi].vertex_count * sizeof(GLuint));
    mesh->faces[fi].vn_indices = 
        (GLuint*)malloc(mesh->faces[fi].vertex_count * sizeof(GLuint));


    /* 12 is the length of each string */
    tokens = (char**)malloc(mesh->faces[fi].vertex_count * sizeof(char*));


    /* split up the line into chunks separated by the null 
     * character so they can be sscanf'ed */
    bufpos = line;
    while(*bufpos != '\0')
    {
        if(*bufpos == ' ')
        {
            *bufpos = '\0';
            if(isdigit(*(bufpos+sizeof(char))))
            {
                tokens[i] = bufpos+sizeof(char);
                i++;
            }
        }
        if(*bufpos == '\n')
        {
            *bufpos = '\0';
            break;
        }
        bufpos += sizeof(char);
    }

    for(i=0; i<mesh->faces[fi].vertex_count; i++)
        sscanf(tokens[i], "%u//%u", &mesh->faces[fi].v_indices[i],
                &mesh->faces[fi].vn_indices[i]);


    free(tokens);
    free(line);

}


/* This counts up the number of SINGLE SPACE-separated
 * tokens are in a word */
static GLushort token_count(const char* string)
{
    GLushort i, count;
    i = count = 0;
    

    while(1)
    {
        if(string[i] == '\0')
            break;
        if(string[i] == ' ')
            count++;
        i++;
    }

    return(count+1);
}

static GLuint generate_display_list(mesh_t* mesh)
{
    GLuint list, i;
    GLubyte j;
    material_t *mat;

    list = glGenLists(1);

    glNewList(list, GL_COMPILE);
        glPushAttrib(GL_ENABLE_BIT|GL_LIGHTING_BIT);

        glShadeModel(GL_SMOOTH);


        for(i=0; i<mesh->face_count; i++)
        {
            mat = &mesh->materials[mesh->faces[i].mat_index];

            /* if i can get the lighting values right, i'll
             * put specular and ambient lighting back in *
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->k_specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->k_ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->k_diffuse);
            */

            glColor3fv(mat->k_diffuse);

            glBegin(GL_POLYGON);

            for(j=0; j<mesh->faces[i].vertex_count; j++)
            {
                glNormal3fv(mesh->normals[mesh->faces[i].vn_indices[j]-1]);
                glVertex3fv(mesh->vertices[mesh->faces[i].v_indices[j]-1]);
            }

            glEnd();
        }

        glPopAttrib();

    glEndList();

    return list;
}


void render_exploding(mesh_t* mesh)
{
    GLuint i;
    GLubyte j;
    material_t* mat;

    glPushAttrib(GL_ENABLE_BIT|GL_LIGHTING_BIT);

    glShadeModel(GL_SMOOTH);


    for(i=0; i<mesh->face_count; i++)
    {
        glPushMatrix();

        glTranslatef(mesh->normals[mesh->faces[i].vn_indices[0]-1][0] 
                        * mesh->explosion_frame,
                     mesh->normals[mesh->faces[i].vn_indices[0]-1][1] 
                        * mesh->explosion_frame,
                     mesh->normals[mesh->faces[i].vn_indices[0]-1][2] 
                        * mesh->explosion_frame);

        mat = &mesh->materials[mesh->faces[i].mat_index];

        glColor3fv(mat->k_diffuse);

        glBegin(GL_POLYGON);

        for(j=0; j<mesh->faces[i].vertex_count; j++)
        {
            glNormal3fv(mesh->normals[mesh->faces[i].vn_indices[j]-1]);
            glVertex3fv(mesh->vertices[mesh->faces[i].v_indices[j]-1]);
        }

        glEnd();

        glPopMatrix();
    }

    /* The nudge factors for the explosion speed are down
     * here.  Adjust away. */
    mesh->explosion_frame+=3.0f*player.collision_speed*player.collision_speed;
    if(mesh->explosion_frame>30.0f*player.collision_speed) 
    {
        mesh->explosion_frame=0.0;
        player.exploding = 0;
        ship_restart(&player);
    }

    glPopAttrib();
}


void mesh_dump(mesh_t* mesh)
{
    GLuint i;
    GLushort j;
    material_t *mat;

    for(i=0; i<mesh->face_count; i++)
    {
        mat = &mesh->materials[mesh->faces[i].mat_index];

        printf("Face %i (of %i)\n\tmaterial: %s\n", i+1, mesh->face_count, mat->name);
        printf("\t\tShininess: %.2f\n\t\tAlpha: %.2f\n", mat->shininess, mat->alpha);
        printf("\t\tDiffuse: R=%.2f G=%.2f B=%.2f\n",
                mat->k_diffuse[0],
                mat->k_diffuse[1],
                mat->k_diffuse[2]);
        printf("\t\tAmbient: R=%.2f G=%.2f B=%.2f\n",
                mat->k_ambient[0],
                mat->k_ambient[1],
                mat->k_ambient[2]);
        printf("\t\tSpecular: R=%.2f G=%.2f B=%.2f\n",
                mat->k_specular[0],
                mat->k_specular[1],
                mat->k_specular[2]);

        printf("\tVertex data:\n");
        for(j=0; j<mesh->faces[i].vertex_count; j++)
        {
            printf("\t\t(%hu/%hu): ", j+1, mesh->faces[i].vertex_count);  
            printf("vertex: (%.2f, %.2f, %.2f) normal: (%.2f, %.2f, %.2f)\n",
                    mesh->normals[mesh->faces[i].v_indices[j]][0],
                    mesh->normals[mesh->faces[i].v_indices[j]][1],
                    mesh->normals[mesh->faces[i].v_indices[j]][2],
                    mesh->normals[mesh->faces[i].vn_indices[j]][0],
                    mesh->normals[mesh->faces[i].vn_indices[j]][1],
                    mesh->normals[mesh->faces[i].vn_indices[j]][2]);
        }

        printf("===========================================================\n");

    }
}

static GLshort parse_mtl_file(char* name, material_t** materials)
{
    FILE* f;
    char filename[256];
    char *at, *pos;
    char buf[90];
    GLushort line = 0;
    GLshort mat_count;
    GLshort mat_index = -1;


    mat_count = 0; 

    prepend_path(MODEL_DIR, name, filename);
    strcat(filename, ".mtl");

    f = fopen(filename, "r");

    while(fgets(buf, 90, f))
    {
        line++;
        if((at = strstr(buf, "newmtl")) != NULL)
        {
            if(buf == at) /* this better happen at the start of the line... */
            {
                mat_count++;
                continue;
            }
            else
            {
                for(pos = buf; pos != at; pos += sizeof(char))
                {
                    if(*pos == '#')
                        break;

                    if(pos+sizeof(char) == at)
                    {
                        fprintf(stderr, "Parse error at line %u in %s!\n", 
                                line, filename);
                        fclose(f);
                        return -1;

                    }
                }
            }
        }
    }

    if(mat_count == 0)
    {
        fclose(f);
        return -1;
    }

    rewind(f);

    *materials = (material_t*)malloc(mat_count*sizeof(material_t));

    while(fgets(buf, 90, f))
    {
        if(isspace(buf[0]) || buf[0] == '#')
            continue;

        if(strncmp(buf, "newmtl", 6) == 0)
        {
            mat_index++;
            sscanf(buf, "newmtl %s", &(*materials)[mat_index].name);
            strcpy((*materials)[mat_index].name, &buf[7]);
            continue;
        }

        if(mat_index == -1)
        {
            fprintf(stderr, "No material selected at this point!\n");
            fclose(f);
            return -1;
        }

        if(strncmp(buf, "Ns", 2) == 0)
            sscanf(buf, "Ns %f", &(*materials)[mat_index].shininess);
        if(strncmp(buf, "d", 1) == 0)
            sscanf(buf, "d %f", &(*materials)[mat_index].alpha);
        if(strncmp(buf, "Kd", 2) == 0)
            sscanf(buf, "Kd %f %f %f", 
                    &(*materials)[mat_index].k_diffuse[0],
                    &(*materials)[mat_index].k_diffuse[1],
                    &(*materials)[mat_index].k_diffuse[2]);
        if(strncmp(buf, "Ka", 2) == 0)
            sscanf(buf, "Ka %f %f %f", 
                    &(*materials)[mat_index].k_ambient[0],
                    &(*materials)[mat_index].k_ambient[1],
                    &(*materials)[mat_index].k_ambient[2]);
        if(strncmp(buf, "Ks", 2) == 0)
            sscanf(buf, "Ks %f %f %f", 
                    &(*materials)[mat_index].k_specular[0],
                    &(*materials)[mat_index].k_specular[1],
                    &(*materials)[mat_index].k_specular[2]);

    }

    fclose(f);


    return mat_count;
}

void destroy_mesh(mesh_t* mesh)
{
    GLuint i;

    for(i=0; i<mesh->face_count; i++)
    {
        free(mesh->faces[i].v_indices);
        free(mesh->faces[i].vn_indices);
    }
    free(mesh->faces);
    free(mesh->vertices);
    free(mesh->normals);
    free(mesh->materials);
}

static GLint material_index(const char* name, mesh_t* m)
{
    GLint i;

    for(i=0; i<m->mat_count; i++)
    {
        if(strcmp(m->materials[i].name, name) == 0)
            return i;
    }
    return -1;
}
/* $Id$ */
