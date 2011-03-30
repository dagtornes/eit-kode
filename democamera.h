#pragma once

#include "demomath.h"

class demoCamera
{
public:
	Matrix4 proj, world, view; 

	void createProj(float fov, float aspect, float near, float far)
	{
		proj = CreatePerspectiveProjection(fov, aspect, near, far);
	}

	void setProj(GLuint shaderProg, const std::string& handle) 
	{
		int loc; 
		const char* cstr = handle.c_str(); 
		loc = glGetUniformLocation(shaderProg, cstr);
		glUniformMatrix4fv(loc, 1, GL_FALSE, proj.c_ptr());
	}
};