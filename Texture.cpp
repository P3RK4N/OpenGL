#include "Texture.h"

Texture::Texture(string name, GLenum type, GLenum socket, GLenum interpolation, GLenum wrap)
{
	imgType = type;
	int width, height, numColCh;
	unsigned char* bytes;
	stbi_set_flip_vertically_on_load(true);
	bytes = stbi_load((char *)name.c_str(), &width, &height, &numColCh, 0);
	glGenTextures(1, &textureID);
	glActiveTexture(socket);
	Bind();

	glTexParameteri(imgType, GL_TEXTURE_MIN_FILTER, interpolation);
	glTexParameteri(imgType, GL_TEXTURE_MAG_FILTER, interpolation);

	glTexParameteri(imgType, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(imgType, GL_TEXTURE_WRAP_T, wrap);

	glTexImage2D(imgType, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(imgType);

	stbi_image_free(bytes);
	Unbind();
	Bind();
}

void Texture::Bind()
{
	glBindTexture(imgType, textureID);
}

void Texture::Unbind()
{
	glBindTexture(imgType, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &textureID);
}