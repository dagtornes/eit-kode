/*
 *  Mesh.h
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MESH_H
#define MESH_H

#include <string>
#include <map>
#include <GL/glew.h>
#include <assimp.hpp>
#include <aiMesh.h>

class Mesh
{
public:
	/**
	 *  Mesh should only contain triangles, use postprocess in assimp to ensure this.
	 */
	Mesh(const aiMesh * mesh);
	
	/**
	 *  vdata, vsize - vertex data and the size of the array
	 *  idata, isize - index data and size
	 */
	Mesh(const GLfloat * vdata, GLsizei vsize, const GLushort * idata, GLsizei isize);
	
	/**
	 *  Position refers to the attribute-location we get from the shader.
	 *  Kind is which primitive to draw.
	 *  Size count is the number of vertices.
	 */
	void render(GLuint shader, GLenum kind, GLsizei count) const;
	
private:
	GLuint make_buffer(GLenum target, GLsizei size, const void * data);
	
	GLuint vertex_buffer, index_buffer;
//	GLuint shader_location;
};

#endif