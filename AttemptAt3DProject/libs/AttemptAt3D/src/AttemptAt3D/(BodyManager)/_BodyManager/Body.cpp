
#include "AttemptAt3D/(BodyManager)/_BodyManager/Body.hpp"

#include "AttemptAt3D/(Debug)/Debug.hpp"

namespace AttemptAt3D::_BodyManager
{
	/* Constructors */

	Body::Body(const Trans* trans, const Mesh* mesh) :
		trans(trans),
		mesh(mesh),
		vao(-1),
		vbo(-1),
		ebo(-1)
	{}

	/* Methods */

	void Body::initializeVao(ShaderManager& shaderManager)
	{
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

		glBufferData(GL_ARRAY_BUFFER, this->mesh->verticesLen * sizeof(this->mesh->vertices[0]), this->mesh->vertices.get(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->elementsLen * sizeof(this->mesh->elements[0]), this->mesh->elements.get(), GL_DYNAMIC_DRAW);

		shaderManager.doAttribs();

		glBindVertexArray(0);
	}

	void Body::drawBody(ShaderManager& shaderManager)
	{
		glBindVertexArray(this->vao);

		// Debug::Printf("transform = %s\n", this->trans->toString().c_str());
		shaderManager.set_uni_transScaleVal(this->trans->scale.matrix());
		shaderManager.set_uni_transRotVal(this->trans->rot.matrix());
		shaderManager.set_uni_transPosVal(this->trans->pos.matrix());
		glDrawElements(GL_TRIANGLES, this->mesh->elementsLen, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void Body::cleanupForGl()
	{
		glDeleteBuffers(1, &this->ebo);
		glDeleteBuffers(1, &this->vbo);
		glDeleteVertexArrays(1, &this->vao);
	}
}
