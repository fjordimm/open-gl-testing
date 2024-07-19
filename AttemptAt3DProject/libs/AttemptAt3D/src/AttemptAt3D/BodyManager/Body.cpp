
#include "AttemptAt3D/BodyManager/Body.hpp"

#include "AttemptAt3D/Debug/Debug.hpp"

namespace AttemptAt3D::_BodyManager
{
	/* Constructors */

	Body::Body() :
		vao(-1),
		vbo(-1),
		ebo(-1),
		verticesLen(0),
		vertices(nullptr),
		elementsLen(0),
		elements(nullptr)
	{}

	/* Methods */

	void Body::setData(std::size_t verticesLen, std::unique_ptr<float[]> vertices, std::size_t elementsLen, std::unique_ptr<GLuint[]> elements)
	{
		this->verticesLen = verticesLen;
		this->vertices = std::move(vertices);
		this->elementsLen = elementsLen;
		this->elements = std::move(elements);
	}

	void Body::doGlGens()
	{
		Debug::Log("doGlGens");
		// glGenVertexArrays(1, &this->vao);
		// glGenBuffers(1, &this->vbo);
		// glGenBuffers(1, &this->ebo);

		// Temp
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		
		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	}

	void Body::attachDataToGlBuffers()
	{
		Debug::Log("attachDataToGlBuffers");
		// glNamedBufferData(this->vbo, this->verticesLen * sizeof(this->vertices[0]), this->vertices.get(), GL_STATIC_DRAW);
		// glNamedBufferData(this->ebo, this->elementsLen * sizeof(this->elements[0]), this->elements.get(), GL_STATIC_DRAW);
	}

	void Body::bindDataAndDraw()
	{
		// Debug::Log("Verts...");
		// for (int i = 0; i < this->verticesLen; i++)
		// {
		// 	Debug::Printf("  %f\n", this->vertices[i]);
		// }

		// Debug::Log("Elems...");
		// for (int i = 0; i < this->elementsLen; i++)
		// {
		// 	Debug::Printf("  %i\n", this->elements[i]);
		// }

		// glBindVertexArray(this->vao);
		// glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

		// Temp
		glBufferData(GL_ARRAY_BUFFER, this->verticesLen * sizeof(this->vertices[0]), this->vertices.get(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementsLen * sizeof(this->elements[0]), this->elements.get(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, 6 * 4, GL_UNSIGNED_INT, 0);
	}

	void Body::cleanupForGl()
	{
		glDeleteBuffers(1, &this->ebo);
		glDeleteBuffers(1, &this->vbo);
		glDeleteVertexArrays(1, &this->vao);
	}
}
