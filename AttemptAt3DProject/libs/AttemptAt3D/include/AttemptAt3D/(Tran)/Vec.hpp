
#pragma once

#include <string>
#include "AttemptAt3D/(headerGroups)/glmGroup.hpp"

namespace AttemptAt3D
{
	typedef glm::vec3 Vec;

	namespace Vecs
	{
		std::string ToString(const Vec& subj);

		static const Vec Zero = glm::vec3(0.0f, 0.0f, 0.0f);
		static const Vec One = glm::vec3(1.0f, 1.0f, 1.0f);
		static const Vec Forwards = glm::vec3(0.0f, 1.0f, 0.0f);
		static const Vec Right = glm::vec3(1.0f, 0.0f, 0.0f);
		static const Vec Up = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}
