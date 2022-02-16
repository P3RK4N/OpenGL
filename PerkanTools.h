#ifndef PERKAN_TOOLS_CLASS_H
#define PERKAN_TOOLS_CLASS_H

#include<iostream>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include "shaderClass.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"

using namespace std;

GLFWwindow *initialize(char *name, int minor, int major, int windowWidth, int windowHeight);

#endif