#include <stdio.h>

#include "demosys.h" 
#include "sb_main.h" 

int main()
{
	printf("Entry main\n"); 
	
	displayWindow dispW(true); 

	sandboxScene scene1; 

	int currentScene = 0; 
	float time = 0.f; 
	while( !glfwGetKey(GLFW_KEY_ESC) )
	{
		
		switch (currentScene)
		{
		case 0:
			scene1.render(time); 
			break; 
		default:
			scene1.render(time); 
			break; 
		}

	
		if(glfwGetKey(GLFW_KEY_RIGHT))
			currentScene++;
		if(glfwGetKey(GLFW_KEY_LEFT))
			currentScene--;
		printf("CurrentScene = %d\r", currentScene); 

	}
	return 1; 
};