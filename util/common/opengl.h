﻿/*****************************************************************//**
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
/* GLM */
#include "glm.h"
/* STL */
#include "standard.h"
/* Assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/* SDL */
#include "sdl2.h"
/* Dear ImGui */
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_sdl2.h"
#include "../../imgui/imgui_impl_opengl3.h"
/* LOG */
#include "../logger/log.h"
/* ENTT */
#include <entt/entt.hpp>

#ifdef DEBUG
#define GL_CALL(func)  func;checkError();
#else
#define GL_CALL(func)  func;
#endif

void checkError();

#endif // OPENGL_H