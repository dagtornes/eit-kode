/*
 *  generator.cpp
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "generator.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

float random(float range)
{
	int r = rand();
	return range * r / RAND_MAX;
}

float noisei(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float noisef(float x, float y)
{
	float corners = (noisei(x-1, y-1) + noisei(x+1, y-1) + noisei(x-1, y+1) + noisei(x+1, y+1)) / 16;
	float sides   = (noisei(x  , y-1) + noisei(x,   y+1) + noisei(x-1, y  ) + noisei(x+1, y  )) / 8;
	float center  = noisei(x, y) / 4;
	return corners + sides + center;
}

float noisel(float x, float y)
{
	int ix = x, iy = y;
	float fx = x - ix, fy = y - iy;
	
	float v1 = noisef(ix  , iy);
	float v2 = noisef(ix+1, iy);
	float v3 = noisef(ix  , iy+1);
	float v4 = noisef(ix+1, iy+1);
	float i1 = lerp(v1, v2, fx);
	float i2 = lerp(v3, v4, fx);
	return lerp(i1, i2, fy);
}

float noisep(float x, float y)
{
	float r = 0.0;
	float p = 0.25;
	int n = 8;
	for (int i = 0; i != n; ++i) {
		float f = std::pow(2.0f, (float) i);
		float a = std::pow(p, (float) i);
		r += a * noisel(x * f, y * f);
	}
	return r;
}

float lerp(float a, float b, float t)
{
	return (1-t) * a + t * b;
}


Mesh generate(int w, int h, int seed)
{
	GLfloat* heights = new GLfloat[w*h];
	for (int i = 0; i != w*h; ++i)
		heights[i] = noisep(i / w, i % w);
	
	float size = 16.0;
	float sx = -size/2, ix = size / (w-1);
	float sz = -size/2, iz = size / (h-1);
	int s = (w-1)*(h-1);
	GLfloat* vertices = new GLfloat[8*w*h];
	GLushort* indices = new GLushort[6*s];
	
	for (int z = 0; z != h; ++z) {
		for (int x = 0; x != w; ++x) {
			int i = z*w + x;
			vertices[8*i+0] = sx + ix * x;
			vertices[8*i+1] = heights[z*h+x];
			vertices[8*i+2] = sz + iz * z;
			
			vertices[8*i+3] = (1.0/(w-1)) * x;
			vertices[8*i+4] = (1.0/(h-1)) * z;
			
			vertices[8*i+5] = 0.0;
			vertices[8*i+6] = 0.0;
			vertices[8*i+7] = 0.0;
		}
	}
	for (int z = 0; z != h-1; ++z) {
		for (int x = 0; x != w-1; ++x) {
			int i = z * (w-1) + x;
			GLushort i2 = z * w + x;
			indices[6*i+0] = i2;
			indices[6*i+1] = i2 + w;
			indices[6*i+2] = i2 + 1;
			
			indices[6*i+3] = i2 + 1;
			indices[6*i+4] = i2 + w;
			indices[6*i+5] = i2 + w + 1;
		}
	}
	Mesh r(vertices, 8*w*h, indices, 6*s);
	
	delete [] vertices;
	delete [] indices;
	delete [] heights;
	return r;
}