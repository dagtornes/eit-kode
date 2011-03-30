#pragma once

#include <vector> 
#include "cml\cml.h"
#include "demomath.h" 

class meshObject
{
public: 
	void createObject(char *handle) //int numVert, int sizeVert, void *vertBuffer, int numIndices, GLushort *indBuffer
	{
	/*
		if(vertBuffer == NULL) {
			printf("createObject vertBuffer==NULL::%s\n", handle);
			return;
		}
		
		if(indBuffer == NULL) {
			printf("createObject indBuffer==NULL::%s\n", handle); 
			return; 
		}
	*/
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*this->vertices, &vertexBuffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID); 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*this->indices, &indexBuffer[0], GL_STATIC_DRAW);

		printf("created Object :: %s\n", handle);
	}

public: 
	GLuint vertexBufferID; 
	GLuint vertexArrayID;
	GLuint indexBufferID; 
	GLuint indexArrayID; 

	int vertices, faces, indices;
	int vertexSize; //holds number of floats  xyz = 3 ...
	GLushort*       indexBuffer; 
	GLfloat*	    vertexBuffer; 

	meshObject() { vertices = 0; faces = 0; indices = 0; indexBuffer = NULL; indexBuffer = NULL; }
	~meshObject() {
		if (vertexBuffer != NULL)
			delete [] vertexBuffer;
		if (indexBuffer != NULL)
			delete [] indexBuffer;
		printf("Mesh Deconstructor\n");
	}

	void setObject(GLuint shaderProg)  //maybe have set shaderProg as global
	{	
		int loc; 
		loc = glGetAttribLocation(shaderProg, "posIn"); //chk -1 if failed
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//setups attibutes and draws the damn thing
	
	void drawObject(GLuint shaderProg)
	{		
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
		glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);

		glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_SHORT, 0);
		//glFlush();

//	    glDisableClientState(GL_VERTEX_ARRAY);
//		glBindBuffer(GL_ARRAY_BUFFER, NULL);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}	



		//generates a triangle
	void initTri()
	{
		this->vertices = 3; 
		this->vertexSize = 3; 
		this->indices = 3;
		this->faces = 1; 

		indexBuffer = new GLushort [this->indices]; 
		vertexBuffer = new GLfloat [this->vertexSize*this->vertices];

#if 0
		GLfloat triangleVertex[]  = { 0.0f, 0.5f, -2.0f, 
							       -0.5f, -0.5f, -2.0f,
			   		               0.5f, -0.5f, -2.0f };
#endif 
		GLfloat triangleVertex[]  = { 1.0f, 1.0f, -1.0f, 
							          -1.0f, 1.0f, -1.0f,
			   		               -1.f, -1.f, -1.0f };
		GLushort triangleIndex[] = { 0, 1, 2 }; 

		memcpy(vertexBuffer, &triangleVertex, sizeof(GLfloat)*this->vertexSize*this->vertices);
		memcpy(indexBuffer, &triangleIndex, sizeof(GLushort)*this->indices);

		this->createObject("triangle");
	} 

	void initScreenSpace()
	{
		this->vertices = 4; 
		this->vertexSize = 3; 
		this->indices = 6; 
		this->faces = 2; 

		indexBuffer = new GLushort [this->indices]; 
		vertexBuffer = new GLfloat [this->vertexSize*this->vertices]; 

		GLfloat screenspaceVertex[] = { -1.f,  1.f, -2.f, 
										 1.f,  1.f, -2.f,
										-1.f, -1.f, -2.f,
										 1.f, -1.f, -2.f };
		GLushort screenspaceIndex[] = {0, 2, 1,
									   1, 3, 2 };

		memcpy(vertexBuffer, &screenspaceVertex, sizeof(GLfloat)*this->vertexSize*this->vertices);
		memcpy(indexBuffer, &screenspaceIndex, sizeof(GLushort)*this->indices);
		this->createObject("screenspace");
	}
}; 

/*
void generatePlane(int x, int y) 
void generateGrid */