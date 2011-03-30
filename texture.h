#pragma once

/*
Supported formats:
png

Fixed state? 
*/

class textureObject
{
public:
	GLuint texture; 


	textureObject() { texture = NULL; }
	~textureObject() { if(texture!=NULL) glDeleteTextures(1, &texture); }

	bool loadTexture(const char *filename) 
	{
		glGenTextures(1, &texture); 
		glBindTexture(GL_TEXTURE_2D, texture); 
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	
		

		return true; 
	}

	

};

//void setTextureState (bilinar point ect