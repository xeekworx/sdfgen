#include "texture.h"

using namespace sdfgen;

static GLuint create_texture(const byte_ptr pixels, const uint32_t width, const uint32_t height, const bool mipmap, const bool anisotropic);

texture::texture(const sdfgen::image& image, const bool mipmap, const bool anisotropic)
	: opengl_object(opengl_object_type::TEXTURE2D)
{
	id(create_texture(image.pixels(), image.width(), image.height(), mipmap, anisotropic));
}

texture::texture(const byte_ptr pixels, const uint32_t width, const uint32_t height, const bool mipmap, const bool anisotropic) 
	: opengl_object(std::string(), 0U, opengl_object_type::TEXTURE2D)
{
	id(create_texture(pixels, width, height, mipmap, anisotropic));
}

texture::~texture()
{
	const GLuint texture_id = id();
	if(texture_id) {
		glDeleteTextures(1, &texture_id);
		unbind();
	}
}

void texture::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id());
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint create_texture(const byte_ptr pixels, const uint32_t width, const uint32_t height, const bool mipmap, const bool anisotropic)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if(!texture_id) {
		throw std::exception(glerror_to_string(glGetError()).c_str());
	}

	// BIND & SET TEXTURE DATA:
	GLenum glerror = glGetError(); // First reset gl's error code.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	if((glerror = glGetError()) != GL_NO_ERROR) {
		glDeleteTextures(1, &texture_id);
		throw std::exception(glerror_to_string(glGetError()).c_str());
	}

	// CONFIGURE TEXTURE & GENERATE MIPMAP:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
		if(anisotropic) {
			GLfloat max_anisotropy = 0;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
		}
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	return texture_id;
}