/*
 *  camera.cpp
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "camera.h"
#include "util.h"

void orbit(float yaw, float pitch, float radius, cml::vector3f origin, GLuint program)
{
	GLuint mv = glGetUniformLocation(program, "mv");
	
	cml::matrix44f_c rot;
	cml::matrix44f_c tra;
	cml::matrix44f_c final;
	cml::matrix_translation(final, 0.0f, 0.0f, radius);
	cml::matrix_rotation_world_y(rot, yaw);
	cml::matrix_rotation_world_x(tra, pitch);
	final *= tra * rot;
	glUniformMatrix4fv(mv, 1, GL_FALSE, final.data());
}

void topdown(cml::vector3f pos, GLuint program)
{
	orbit(0.0f, cml::rad(90.0), 1000.0, pos, program);
}