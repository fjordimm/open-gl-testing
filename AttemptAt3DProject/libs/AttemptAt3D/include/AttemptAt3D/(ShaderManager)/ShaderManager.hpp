
#pragma once

#include <memory>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "AttemptAt3D/(headerGroups)/glmGroup.hpp"

namespace AttemptAt3D
{
	class ShaderManager
	{
		/* Fields */

	   private:
		GLuint vertexShader;
		GLuint geometryShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
		GLint attrib_position;
		GLint attrib_color;
		GLint uni_transScale;
		GLint uni_transRot;
		GLint uni_transPos;
		GLint uni_view;
		GLint uni_proj;
		GLint uni_sunRot;

		/* Getters and Setters */

	   public:
		void set_uni_transScaleVal(const glm::mat4& val);
		void set_uni_transRotVal(const glm::mat4& val);
		void set_uni_transPosVal(const glm::mat4& val);
		void set_uni_viewVal(const glm::mat4& val);
		void set_uni_projVal(const glm::mat4& val);
		void set_uni_sunRotVal(const glm::mat4& val);

		/* Constructors */

	   public:
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
		
		ShaderManager();

		/* Methods */

	   public:
		void compileAndActivateShaders();
		void doAttribs();
		void cleanupForGl();

	   private:
		void checkShaderCompilation(GLuint shader) const;
	};
}
