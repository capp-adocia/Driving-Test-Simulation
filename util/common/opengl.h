/*****************************************************************//**
 * \file   opengl.h
 * \brief  OpenGL相关头文件
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef OPENGL_H
#define OPENGL_H
/* glad */
#include <glad/glad.h>
/* STL */
#include "standard.h"
/* Assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/* GLM */
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
/* SDL */
#include "sdl2.h"
/* Dear ImGui */
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_sdl2.h"
#include "../../imgui/imgui_impl_opengl3.h"

#ifdef DEBUG
#define GL_CALL(func)  func;checkError();
#else
#define GL_CALL(func)  func;
#endif

void checkError();

#endif // OPENGL_H