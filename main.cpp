#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#include <stdio.h>
#include <cmath>
#include <assimp.hpp>
#include <aiscene.h>
#include <aiPostProcess.h>

#include "util.h"
#include "dshaderhandler.h"
#include "Mesh.h"

#include "generator.h"
#include "camera.h"

#include <cml/cml.h>

static struct {
    GLuint textures[2];
    
    struct {
        GLint fade_factor;
        GLint textures[2];
		GLint mv_matrix;
		GLint pr_matrix;
    } uniforms;
	
    struct {
        GLint position;
    } attributes;
	
    GLfloat fade_factor;
} g_resources;

static GLuint make_texture(const char *filename)
{
    int width, height;
    void *pixels = read_tga(filename, &width, &height);
    GLuint texture;
	
    if (!pixels)
        return 0;
	
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0,           /* target, level */
				 GL_RGB8,                    /* internal format */
				 width, height, 0,           /* width, height, border */
				 GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
				 pixels                      /* pixels */
				 );
    free(pixels);
    return texture;
}

static int make_resources(GLuint program)
{
	g_resources.textures[0] = make_texture("data/font512.tga");
    g_resources.textures[1] = make_texture("data/hello2.tga");
	
    if (g_resources.textures[0] == 0 || g_resources.textures[1] == 0)
        return 0;
	
	g_resources.uniforms.fade_factor
	= glGetUniformLocation(program, "fade_factor");
    g_resources.uniforms.textures[0]
	= glGetUniformLocation(program, "textures[0]");
    g_resources.uniforms.textures[1]
	= glGetUniformLocation(program, "textures[1]");
	
	g_resources.uniforms.pr_matrix =
	glGetUniformLocation(program, "projection");
	
	g_resources.uniforms.mv_matrix =
	glGetUniformLocation(program, "mv");
	
    g_resources.attributes.position
	= glGetAttribLocation(program, "position");
	
    return 1;
}

static void update_fade_factor(void)
{
    double milliseconds = glfwGetTime();
    g_resources.fade_factor = sinf((float)milliseconds) * 0.5f + 0.5f;
}

static void bind_textures(GLuint program)
{
    glUseProgram(program);
	
    glUniform1f(g_resources.uniforms.fade_factor, g_resources.fade_factor);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_resources.textures[0]);
    glUniform1i(g_resources.uniforms.textures[0], 0);
	
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_resources.textures[1]);
    glUniform1i(g_resources.uniforms.textures[1], 1);
}



void find_extents(const aiMesh* mesh, cml::vector3f min, cml::vector3f max)
{
	float minx = 0.0, maxx = 0.0;
	float miny = 0.0, maxy = 0.0;
	float minz = 0.0, maxz = 0.0;
	
	for (int i = 0; i != mesh->mNumVertices; ++i) {
		minx = std::min(mesh->mVertices[i].x, minx);
		maxx = std::max(mesh->mVertices[i].x, maxx);
		miny = std::min(mesh->mVertices[i].y, miny);
		maxy = std::max(mesh->mVertices[i].y, maxy);
		minz = std::min(mesh->mVertices[i].z, minz);
		maxz = std::max(mesh->mVertices[i].z, maxz);
	}
	
	min = cml::vector3f(minx, miny, minz);
	max = cml::vector3f(maxx, maxy, maxz);
}

Mesh load_mesh(const std::string& name)
{
	Assimp::Importer imp;
	const aiScene * scene = imp.ReadFile(name, aiProcess_Triangulate);
	std::cout << "Loading " << name << std::endl;
	std::cout << "Meshes: " << scene->mNumMeshes << std::endl;
	return Mesh(scene->mMeshes[0]);
}



int main (int argc, const char * argv[])
{
	std::string vertname("data/simple.v.glsl"), fragname("data/simple.f.glsl");
	if (argc == 3) {
		vertname = argv[1];
		fragname = argv[2];
	}
	
	glfwInit();
	int w = 800, h = 600;
    glfwOpenWindow(w, h, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.4, 0.2, 1.0);
	glClearDepth(1.0);
	
	cml::matrix44f_c proj;
	cml::matrix_perspective_yfov_LH(proj, cml::rad(60.0f), (float) w / h, 1.0f, 100000.f, cml::z_clip_zero);
	
	ShaderHandler simple(vertname, fragname);
	make_resources(simple.getProgram());
	
	simple.useProgram();
	glUniformMatrix4fv(g_resources.uniforms.pr_matrix, 1, GL_FALSE, proj.data());
	
	std::string fname ("data/plan.stl");

	int tw = 32, th = 32;
	Mesh terrain = generate(tw, th, 0);
	int cnt = 6*(tw-1)*(th-1);
	Mesh test2 = load_mesh(fname);
	
	bool running = true;
	float last = glfwGetTime();
	
	float y = 0.0f, p = 0.0f, r = 100.0;
	
	float wait = 0.0;
	while (running) {
		glfwPollEvents();
		float now = glfwGetTime();
		float dt = now - last;
		last = now;
		{
		running = !glfwGetKey(GLFW_KEY_ESC);
		if (glfwGetKey(GLFW_KEY_LEFT))  y += cml::rad(45.0f) * dt;
		if (glfwGetKey(GLFW_KEY_RIGHT)) y -= cml::rad(45.0f) * dt;
		if (glfwGetKey(GLFW_KEY_UP))    p += cml::rad(45.0f) * dt;
		if (glfwGetKey(GLFW_KEY_DOWN))  p -= cml::rad(45.0) * dt;
		if (glfwGetKey('R') && wait <= 0.0) {
			wait = 0.15;
			y = 0.0;
			p = 0.0;
			r = 100.0;
		}
		if (glfwGetKey('T') && wait <= 0.0) {
			wait = 0.15;
			y = 0.0;
			p = cml::rad(90.f);
			r = 100.0;
		}
		if (glfwGetKey('Y') && wait <= 0.0) {
			wait = 0.15;
			y = cml::rad(90.f);
			p = 0.0;
			r = 100.0;
		}
		if (glfwGetKey('Q'))            r -= 0.5 * r * dt;
		if (glfwGetKey('A'))            r += 0.5 * r * dt;
		if (glfwGetKey('W') && wait <= 0.0) {
			cnt += 1; wait = 0.15;
			std::cout << cnt << std::endl;
		}
		if (glfwGetKey('S') && wait <= 0.0 && cnt > 1)	{
			cnt -= 1; wait = 0.15;
			std::cout << cnt << std::endl;
		}
		wait -= dt;
		}
		
		orbit(y, p, r, cml::vector3f(0.0, 0.0, 0.0), simple.getProgram());
//		topdown(cml::vector3f(0.0, 0.0, 0.0), simple.getProgram());
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		update_fade_factor();
		bind_textures(simple.getProgram());

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		test2.render(simple.getProgram(), GL_TRIANGLES, 6);
		terrain.render(simple.getProgram(), GL_TRIANGLES, cnt);
		
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			std::cout << "End of main: " << gluErrorString(err) << '\n';
		
		glfwSwapBuffers();
	}
	
	glfwCloseWindow();
	glfwTerminate();
	return 0;
}
