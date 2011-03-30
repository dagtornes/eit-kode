/*
 *  Mesh.cpp
 *  libtest
 *
 *  Created by Dag Øyvind Tornes on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Mesh.h"
#include <iostream>

Mesh::Mesh(const aiMesh * mesh)
{
	GLfloat* vertices = new GLfloat[mesh->mNumVertices*8];
	for (int i = 0; i != mesh->mNumVertices; ++i) {
		vertices[8*i + 0] = mesh->mVertices[i].x;
		vertices[8*i + 1] = mesh->mVertices[i].y;
		vertices[8*i + 2] = mesh->mVertices[i].z;

		vertices[8*i + 3] = 0.0;
		vertices[8*i + 4] = 0.0;
		vertices[8*i + 5] = 0.0;
		vertices[8*i + 6] = 0.0;
		vertices[8*i + 7] = 0.0;
	}
	vertex_buffer = make_buffer(GL_ARRAY_BUFFER,
								8 * sizeof(GLfloat) * mesh->mNumVertices,
								vertices);
	delete [] vertices;
	
	
	GLushort* indices = new GLushort[mesh->mNumFaces*3];
	for (int i = 0; i != mesh->mNumFaces; ++i) {
		indices[3*i + 0] = mesh->mFaces[i].mIndices[0];
		indices[3*i + 1] = mesh->mFaces[i].mIndices[1];
		indices[3*i + 2] = mesh->mFaces[i].mIndices[2];
	}
	index_buffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER,
							   3 * sizeof(GLushort) * mesh->mNumFaces,
							   indices);
	delete [] indices;
}

Mesh::Mesh(const GLfloat * vdata, GLsizei vsize, const GLushort * idata, GLsizei isize)
{
	vertex_buffer = make_buffer(GL_ARRAY_BUFFER,
								vsize * sizeof(GLfloat),
								vdata);
	index_buffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER,
							   isize * sizeof(GLushort),
							   idata);
}

void Mesh::render(GLuint program, GLenum kind, GLsizei count) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	
	GLuint position = glGetAttribLocation(program, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(position);

	GLuint texcoord = 1; //glGetAttribLocation(shader, "texcoord");
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void *) 3);
	glEnableVertexAttribArray(texcoord);
	
	GLuint normal = 2; //glGetAttribLocation(program, "normal");
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void *) 5);
	glEnableVertexAttribArray(normal);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glDrawElements(kind, count, GL_UNSIGNED_SHORT, (void *) 0);
}

GLuint Mesh::make_buffer(GLenum target, GLsizei size, const void * data)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	
	glBufferData(target, size, data, GL_STATIC_DRAW);
	
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << "Error in make_buffer(): " << gluErrorString(err) << std::endl;
	
	return buffer;
}
