/*****************************************************************//**
 * \file   tools.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef TOOLS_H
#define TOOLS_H
#include "../core.h"

class Tools {
public:
	//传入一个矩阵，结构其中的位置、旋转信息XYZ、缩放信息
	static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
};
#endif // !TOOLS_H