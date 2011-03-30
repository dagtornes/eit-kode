/*
 *  camera.h
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "cml/cml.h"

/**
 *  This function creates a view-matrix, and attaches it to the mv-attribute of the given program.
 */
void orbit(float yaw, float pitch, float radius, cml::vector3f origin, GLuint program);

/**
 *  This function creates a view-matrix looking straight down from pos,
 *  and attaches it to the given program.
 */
void topdown(cml::vector3f pos, GLuint program);

#endif