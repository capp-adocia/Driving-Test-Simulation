/*****************************************************************//**
 * \file   Component.h
 * \brief  组件，用于存储实体对象的数据
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#include "../../../util/common/tools.h"

namespace component {
	struct TagComponent
	{
		std::string Name;
		bool IsEnabled = true;
	};

	struct TransformComponent
	{
		glm::mat4 transform;

		const glm::vec3& position() const { return Util::cast<glm::vec3>(transform[3]); }
		glm::vec3& position() { return Util::cast<glm::vec3>(transform[3]); }
		glm::mat3 basis() const { return glm::mat3(transform); }
		void set_basis(const glm::mat3& basis) { transform = basis; }
		glm::quat quat() const { return glm::quat_cast(basis()); }
	};
	struct PhysicsComponent
	{
		glm::vec3 velocity;
	};

}
#endif // !COMPONENT_H