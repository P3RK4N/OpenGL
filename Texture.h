#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<stb/stb_image.h>
#include<glad/glad.h>
#include<string>

using namespace std;

class Texture
{
	private:
		GLuint textureID;
		GLenum imgType;
	public:
		Texture(string name, GLenum type, GLenum socket, GLenum interpolation, GLenum wrap);
		void Bind();
		void Unbind();
		void Delete();
};
#endif
