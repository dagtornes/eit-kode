#pragma once

#include <string>
#include "demosys.h" 
#include "democamera.h" 
#include "texture.h"
#include "mesh.h" 

/*
todo - priorized task list
-load textures (png, jpg .. ... 
-load meshfiles 
-create and render to fbo's 
-create a resource mananger (HASJmap stylê) 
- synctracker 
- compressed textures
- instancing 
*/

class sandboxScene 
{
public: 
	shaderHandler shDefault; 
	
	demoCamera	  cam; 
	meshObject	  moScreen; 


	sandboxScene() 
	{
		shDefault.createShaderProgram("screenspace.vert", "texcoord.frag");
		//moScreen.initTri();
		moScreen.initScreenSpace(); 
		cam.createProj(90.f, 4.f/3.f, 1.0f, 1000.f);
	}

	void render(float time) 
	{
		glClearColor(0.0, 0.3, 0.0, 0.0);
		glDisable(GL_DEPTH_TEST);
		
		glFrontFace(GL_CCW);
		glCullFace(GL_FRONT_AND_BACK);
		glShadeModel(GL_SMOOTH);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shDefault.useProgram(); 
		cam.setProj(shDefault.getShaderHandle(),"projectionMatrix"); 
   	    moScreen.setObject(shDefault.getShaderHandle());
	    moScreen.drawObject(shDefault.getShaderHandle());

		glfwSwapBuffers();
	}
};