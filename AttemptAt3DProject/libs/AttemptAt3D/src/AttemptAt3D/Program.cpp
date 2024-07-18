
////////////////////////////////////////////////////////////

///// Includes /////

// #define DO_MEMORY_CHECK
// #ifdef DO_MEMORY_CHECK
// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #endif

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "CMakeConfig.h"
#include "AttemptAt3D/FileLoading/FileLoading.h"
#include "AttemptAt3D/Debug/Debug.h"

using namespace AttemptAt3D;

////////////////////////////////////////////////////////////

///// Constants /////

constexpr int INITIAL_WINDOW_WIDTH = 600;
constexpr int INITIAL_WINDOW_HEIGHT = 600;
constexpr char WINDOW_TITLE[] = "Test window";

constexpr char VERTEX_SHADER_SOURCE_PATH[] = "res/shaders/vertexShaderSource.glsl";
constexpr char GEOMETRY_SHADER_SOURCE_PATH[] = "res/shaders/geometryShaderSource.glsl";
constexpr char FRAGMENT_SHADER_SOURCE_PATH[] = "res/shaders/fragmentShaderSource.glsl";

////////////////////////////////////////////////////////////

///// Forward Declarations /////

void checkShaderCompilation(GLuint shader);

////////////////////////////////////////////////////////////

///// Main Function /////

int main(void)
{
	// // valgrind test:
	// int* bruh = new int[5];
	// // delete[] bruh;

	// printf("Is valid pointer? %i\n", _CrtIsValidHeapPointer(bruh));

	// _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
	// _CrtDumpMemoryLeaks();
	// _CrtDbgReport();

	/* Program Variables */

	/* Create the window, initializing GLFW and GLEW */

	GLFWwindow* window;
	{
		if (!glfwInit())
		{ Debug::logFatalError("glfwInit failed."); }

		// Window hints
		glfwWindowHint(GLFW_SAMPLES, 5); // anti-aliasing

		window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			Debug::logFatalError("glfwCreateWindow failed.");
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		glewInit();
	}

	/* OpenGL stuff */

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float verts1[] =
	{
		// X      Y      Z         R    G    B
		 -0.3f, +0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 -0.3f, -0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 +0.3f, +0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 +0.3f, -0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 
		 -0.3f, -0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 +0.0f, +0.0f, +0.0f,     1.0f,0.0f,0.0f,
		 +0.3f, -0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 
		 +0.3f, -0.3f, +0.8f,     1.0f,0.0f,0.0f,
		 +0.0f, +0.0f, +0.0f,     1.0f,0.0f,0.0f,
		 +0.3f, +0.3f, +0.8f,     1.0f,0.0f,0.0f,
	};

	float verts2[] =
	{
		// X      Y      Z         R    G    B
		 -0.3f, +0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 -0.3f, -0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 +0.3f, +0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 +0.3f, -0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 
		 +0.3f, +0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 +0.0f, +0.0f, +0.0f,     0.0f,1.0f,0.0f,
		 -0.3f, +0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 
		 -0.3f, +0.3f, +0.8f,     0.0f,1.0f,0.0f,
		 +0.0f, +0.0f, +0.0f,     0.0f,1.0f,0.0f,
		 -0.3f, -0.3f, +0.8f,     0.0f,1.0f,0.0f,
	};

	GLuint elems1[] =
	{
		0, 1, 2,
		3, 2, 1,

		4, 5, 6,

		7, 8, 9
	};

	GLuint vao2;
	GLuint vbo2;
	GLuint ebo2;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLint posAttrib;
	GLint colAttrib;
	GLint uniModel;
	GLint uniView;
	GLint uniProj;
	GLint uniSunRot;
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// glNamedBufferData(vbo, sizeof(verts1), verts1, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts1), verts1, GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems1), elems1, GL_STATIC_DRAW);

		const char* vertexShaderSource = FileLoading::loadFile(VERTEX_SHADER_SOURCE_PATH);
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		checkShaderCompilation(vertexShader);
		delete[] vertexShaderSource;

		const char* geometryShaderSource = FileLoading::loadFile(GEOMETRY_SHADER_SOURCE_PATH);
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
		glCompileShader(geometryShader);
		checkShaderCompilation(geometryShader);
		delete[] geometryShaderSource;

		const char* fragmentShaderSource = FileLoading::loadFile(FRAGMENT_SHADER_SOURCE_PATH);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		checkShaderCompilation(fragmentShader);
		delete[] fragmentShaderSource;

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, geometryShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

		colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		uniModel = glGetUniformLocation(shaderProgram, "model");
		// glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		uniView = glGetUniformLocation(shaderProgram, "view");
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, -5.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		uniProj = glGetUniformLocation(shaderProgram, "proj");
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f /*aspect ratio*/, 1.0f, 10.0f);
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		uniSunRot = glGetUniformLocation(shaderProgram, "sunRot");
		glm::quat sunQuatX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 sunRotX = glm::toMat4(sunQuatX);
		glm::quat sunQuatY = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 sunRotY = glm::toMat4(sunQuatY);
		glUniformMatrix4fv(uniSunRot, 1, GL_FALSE, glm::value_ptr(sunRotY * sunRotX));
	}

	/* Main Window Loop */

	auto startTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
    {
		/* Process inputs */

        /* Render stuff */

		auto nowTime = std::chrono::high_resolution_clock::now();
		float timeElapsed = std::chrono::duration_cast<std::chrono::duration<float>>(nowTime - startTime).count();
		
		{
			glm::mat4 model = glm::rotate(glm::mat4(1.0f), timeElapsed * glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		}

		{
			glm::quat sunQuatX = glm::angleAxis(timeElapsed * glm::radians(400.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 sunRotX = glm::toMat4(sunQuatX);
			glm::quat sunQuatY = glm::angleAxis(timeElapsed * glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 sunRotY = glm::toMat4(sunQuatY);
			glUniformMatrix4fv(uniSunRot, 1, GL_FALSE, glm::value_ptr(sunRotY * sunRotX));
		}

		{
			// if (timeElapsed > 2.1f)
			// {
			// 	glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
			// }
		}

		glClearColor(0.1f, 0.0f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts1), verts1, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6 * 4, GL_UNSIGNED_INT, 0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6 * 4, GL_UNSIGNED_INT, 0);

		/* Adjust the canvas when the window is resized */

		glfwSetWindowSizeCallback(window, [](GLFWwindow* _window, int _width, int _height)
		{ glViewport(0, 0, _width, _height); });

        /* Do stuff required by GLFW */

		glfwPollEvents();
        glfwSwapBuffers(window);
    }

	/* Deletions and Termination of GLFW */

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
    glfwTerminate();

	// // valgrind test:
	// int* bruh = new int[5];

	// _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	// _CrtDumpMemoryLeaks();
    return 0;
}

////////////////////////////////////////////////////////////

///// Helpers /////

void checkShaderCompilation(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status)
	{ return; }
	else
	{
		static char buf[512];
		glGetShaderInfoLog(shader, 512, nullptr, buf);
		std::fprintf(stderr, "=== Shader did not compile ===\n");
		std::fprintf(stderr, "%s\n", buf);
		std::exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////
