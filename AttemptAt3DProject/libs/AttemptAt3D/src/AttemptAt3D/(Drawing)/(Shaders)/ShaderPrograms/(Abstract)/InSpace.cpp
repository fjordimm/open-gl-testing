
#include "AttemptAt3D/(Drawing)/(Shaders)/ShaderPrograms/(Abstract)/InSpace.hpp"

#include "AttemptAt3D/(Debug)/Debug.hpp"

namespace AttemptAt3D::ShaderPrograms
{
	/* Constructors */

	InSpace::InSpace() : 
		attribPosition(-1),
		uniView(-1),
		uniProj(-1),
		uniSunRot(-1)
	{}

	/* Getters & Setters */

	void InSpace::setUniView(const glm::mat4& val) const
	{ glUniformMatrix4fv(this->uniView, 1, GL_FALSE, glm::value_ptr(val)); }

	void InSpace::setUniProj(const glm::mat4& val) const
	{ glUniformMatrix4fv(this->uniProj, 1, GL_FALSE, glm::value_ptr(val)); }

	void InSpace::setUniSunRot(const glm::mat4& val) const
	{ glUniformMatrix4fv(this->uniSunRot, 1, GL_FALSE, glm::value_ptr(val)); }

	/* Methods */

	MeshSamples::MeshSample::MeshVertAttribs InSpace::attribFlagsForMeshSamples() const
	{
		return MeshSamples::MeshSample::MeshVertAttribs::Positions3D | this->attribFlagsForMeshSamples_();
	}

	void InSpace::enableAttribsForMesh()
	{
		glEnableVertexAttribArray(this->attribPosition);
		glVertexAttribPointer(this->attribPosition, 3, GL_FLOAT, GL_FALSE, this->getStride(), 0);

		this->enableAttribsForMesh_();
	}

	std::size_t InSpace::getStride()
	{
		return 3 * sizeof(float) + this->getStride_();
	}

	void InSpace::setupUniforms()
	{
		this->uniView = glGetUniformLocation(this->program, "uni_View");
		glm::mat4 uniView_val = glm::lookAt(
			glm::vec3(0.0f, -15.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(this->uniView, 1, GL_FALSE, glm::value_ptr(uniView_val));

		this->uniProj = glGetUniformLocation(this->program, "uni_Proj");
		glm::mat4 uniProj_val = glm::perspective(glm::radians(45.0f), 1.0f /*aspect ratio*/, 1.0f, 100000.0f);
		glUniformMatrix4fv(this->uniProj, 1, GL_FALSE, glm::value_ptr(uniProj_val));

		this->uniSunRot = glGetUniformLocation(this->program, "uni_SunRot");
		glm::mat4 uniSunRot_val = glm::mat4(1.0f);
		glUniformMatrix4fv(this->uniSunRot, 1, GL_FALSE, glm::value_ptr(uniSunRot_val));

		this->setupUniforms_();
	}

	void InSpace::setupAttributes()
	{
		this->attribPosition = glGetAttribLocation(this->program, "attrib_Position");

		this->setupAttributes_();
	}

	void InSpace::updateUniformsFromTran(Tran& tran) const
	{
		this->updateUniformsFromTran_(tran);
	}
}
