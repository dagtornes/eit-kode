#pragma once

#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <fstream>


/* setups a window and gl context */
class displayWindow
{
public: 
	displayWindow(bool devmode) 
	{
		//check ogl support
		int major, minor, rev;  
		glfwGetVersion( &major, &minor, &rev );  
		printf("GLFW Version %d.%d.%d loaded\n", major, minor, rev);
	
		if(!glfwInit()) { printf("glfwInit() failed....\n"); return; }
		
		//prompt usable videomodes
		GLFWvidmode dtop, modes[128]; 
		glfwGetDesktopMode(&dtop);
		printf("Desktop: %dx%d rgb%d%d%d\n", dtop.Width, dtop.Height, dtop.RedBits, dtop.GreenBits, dtop.BlueBits);
		
		int maxVidModes = glfwGetVideoModes(modes, 128);
		for(int i=0;i<maxVidModes;i++)
		{
			//discard lame modes
			if(modes[i].RedBits == 8 && modes[i].Width > 1279)
			printf("Mode[%d]: %dx%d\n",i, modes[i].Width, modes[i].Height);
		}

		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
   		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

		if(devmode)
		{
			if(!glfwOpenWindow(640, 480, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
			{
				printf("glfwOpenWindow() cud nae opan window...\n");
				return;
			}
		} else {
			if(!glfwOpenWindow(dtop.Height, dtop.Width, 8, 8, 8, 8, 24, 8, GLFW_FULLSCREEN))
			{
				printf("glfwOpenWindow() cud nae opan window...\n");
				return ;
			}
		}

		glfwEnable(GLFW_STICKY_KEYS);
		if(glewInit()) { printf("glewInit() failed....\n"); return ; }
		if(GLEW_VERSION_2_0) printf("OpenGL 2.0 Supported\n"); 
	}

	~displayWindow()
	{
		glfwCloseWindow(); 
		glfwTerminate();
		printf("displayWindow Deconstructor\n");
	}
}; 

class shaderHandler
{
public:
	GLuint vertexShader, fragShader, progShader;


//	shaderHandler(const std::string &vshader, const std::string &pshader) {init(vshader, pshader);}
//	shaderHandler(char &vshader, char &pshader) {init(vshader, pshader);}
//	shaderHandler() {}
//	shaderHandler(const char* vert, const char* frag) 
//	{
//		printf("vert = %s\nfrag = %s\n", vert, frag); 
//	}

	void useProgram()  { glUseProgram(progShader); }
	GLuint getShaderHandle() const { return progShader; }


	void createShaderProgram(const std::string &vshader, const std::string &pshader) 
	{
		//Create empty shader objects 
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		progShader = glCreateProgram();
		
		//load -> add name 
		loadShader(vertexShader, vshader);
		loadShader(fragShader, pshader); 

		//compile
		const char* vsh = vshader.c_str();
		const char* psh = pshader.c_str();
		printf("Compiling %s and %s\n", vsh, psh);
		glCompileShader(vertexShader);
		glCompileShader(fragShader);

		//link
		glAttachShader(progShader, vertexShader);
		glAttachShader(progShader, fragShader);
		glLinkProgram(progShader);
		printShaderLog(progShader); 
	}

	void release()  { }
	void loadShader(GLuint object, const std::string& fname) 
	{
		std::string shaderSource;
		const char* cstr;
		shaderSource = LoadShaderFromFile(fname);
		if(shaderSource.length() < 1) 
		{
			printf("Empty shaderfile: %s\n", fname.c_str()); 
			exit(-1); 
		}
		cstr = shaderSource.c_str();
		glShaderSource(object, 1, &cstr, NULL);
	}
	void printShaderLog(GLuint obj)
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
		else 
			printf("Compilatioin was a Success!\n");
	}

	static std::string LoadShaderFromFile(const std::string& name)
	{
		std::string ret, line;
		std::ifstream f(name.c_str());
		while(std::getline(f, line))
		ret += line + std::string("\n");
		return ret;
	}


};


