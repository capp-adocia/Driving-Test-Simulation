/*****************************************************************//**
 * \file   shader.h
 * \brief  对shader进行编译链接
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include "../../util/common/opengl.h"
#include<string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	
	void begin();//开始使用当前Shader

	void end();//结束使用当前Shader

	inline GLuint getProgram() { return mProgram; }

	void setFloat(const std::string& name, float value);

	void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, const float* values);
	void setVector3(const std::string& name, const glm::vec3 value);

	void setInt(const std::string& name, int value);

	void setMatrix4x4(const std::string& name, glm::mat4 value);
	void setMatrix3x3(const std::string& name, glm::mat3 value);
	void setMatrix4x4Array(const std::string& name, glm::mat4* value, unsigned int count);
private:
	//shader program
	//type:COMPILE LINK
	void checkShaderErrors(GLuint target,std::string type);

private:
	GLuint mProgram;
};
#endif // !SHADER_H