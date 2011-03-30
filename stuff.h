
#ifndef STUFF_H
#define STUFF_H

#include <GL/glew.h>
#include "dshaderhandler.h"

void initgl(float w, float h)
{
	glClearColor(.1, .2, .1, 1.0);
}


void test_buffers(GLuint v, GLuint i)
{
	glBindBuffer(GL_ARRAY_BUFFER, v);
	GLfloat z = 0.0;
	GLfloat vdata[] = {-1.0, -1.0, z,
					    1.0, -1.0, z,
					    1.0,  1.0, z,
					   -1.0,  1.0, z};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vdata), vdata, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	GLuint idata[] = {0, 1, 2, 3};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idata), idata, GL_STATIC_DRAW);
}

void test_render2(GLuint v, GLuint i, ShaderHandler & shader)
{
	glUseProgram(shader.getProgram());
	
    glBindBuffer(GL_ARRAY_BUFFER, v);
	GLint position = glGetAttribLocation(shader.getProgram(), "position");
    glVertexAttribPointer(position,                         /* attribute */
						  3,                                /* size */
						  GL_FLOAT,                         /* type */
						  GL_FALSE,                         /* normalized? */
						  sizeof(GLfloat)*3,                /* stride */
						  (void*)0                          /* array buffer offset */
						  );
    glEnableVertexAttribArray(position);
	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
    glDrawElements(
				   GL_TRIANGLE_STRIP,  /* mode */
				   4,                  /* count */
				   GL_UNSIGNED_SHORT,  /* type */
				   (void*)0            /* element array buffer offset */
				   );
	
    glDisableVertexAttribArray(position);
}

void test_render(GLuint v, GLuint i, ShaderHandler & shader)
{
	shader.useProgram();
	
	glBindBuffer(GL_ARRAY_BUFFER, v);
	GLint position = glGetAttribLocation(shader.getProgram(), "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (void *) 0);
	glEnableVertexAttribArray(position);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void *) 0);
	glDisableVertexAttribArray(position);
}

#endif