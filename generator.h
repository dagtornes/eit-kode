/*
 *  generator.h
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "Mesh.h"

/**
 * Generates a VBO containing a "terrain"
 */
Mesh generate(int w, int h, int seed);

float noisef(float x, float y);
float noisei(int x, int y);
float noisel(float x, float y);

float lerp(float a, float b, float t);

#endif