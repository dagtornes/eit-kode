/*
 use shader
 set attributes  (pos, norm, tc) 
 set uniforms    (matrices and other fancysmancy stuff) 
 ->good to go (set geometry data -> attributes) 
 */

#ifndef DSHADERHANDLER_H
#define DSHADERHANDLER_H

#include <iostream>
#include "dio.h"

class ShaderHandler 
{
public:
	ShaderHandler(const std::string &vshader, const std::string &pshader) {init(vshader, pshader);}
	void useProgram()  { glUseProgram(progShader); }
	GLuint getProgram() const {return progShader;}

private:
	void init(const std::string &vshader, const std::string &pshader) 
	{
		//Create empty shader objects 
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		progShader = glCreateProgram();
		
		//load -> add name 
		loadShader(vertexShader, vshader);
		loadShader(fragShader, pshader); 

		GLint status;
		
		//compile
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (!status)
			std::cout << "Failed to compile vertex shader" << std::endl;
		myPrintLog(vertexShader);
		
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
		if (!status)
			std::cout << "Failed to compile fragment shader" << std::endl;
		myPrintLog(fragShader);

		//link
		glAttachShader(progShader, vertexShader);
		glAttachShader(progShader, fragShader);
		glLinkProgram(progShader);
		glGetProgramiv(progShader, GL_LINK_STATUS, &status);
		if (!status)
			std::cout << "Failed to link shaders" << std::endl;
		myPrintLog(progShader); 
	}

	void release()  { }
	
	void loadShader(GLuint object, const std::string& fname) 
	{
		std::string shaderSource;
		int length;
		const GLchar * cstr = (const GLchar *) file_contents(fname.c_str(), &length);
		
		if(length < 1) 
		{
			printf("Empty shaderfile: %s\n", fname.c_str()); 
			exit(-1); 
		}
		glShaderSource(object, 1, &cstr, NULL);
	}
	
	void myPrintLog(GLuint obj)
	{
		int infologLength = 0;
		int maxLength;
		
		if(glIsShader(obj))
			glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
		else
			glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
		
		char infoLog[512];
		
		if (glIsShader(obj))
			glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
		else
			glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
		
		if (infologLength > 0)
			printf("%s\n",infoLog);
	}
	
	GLuint vertexShader, fragShader, progShader;
};

#endif