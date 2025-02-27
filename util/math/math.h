/*****************************************************************//**
 * \file   math.h
 * \brief  数学工具
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef MATH_H
#define MATH_H

#include "../common/glm.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MTools
{
	// 传入一个矩阵，结构其中的位置、旋转信息XYZ、缩放信息
	void Decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);

}
#endif // MATH_H