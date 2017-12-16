#include <algorithm>
#include <iterator>
#include "vertex_buffer.h"

using namespace sdfgen;

vertex_buffer::vertex_buffer(const float * data, const size_t size, const unsigned int components) 
	: opengl_object(opengl_object_type::VBO), m_components(components)
{
	GLuint vbo_id = 0;
	glGenBuffers(1, &vbo_id);
	id(vbo_id);
	m_size = size;
	m_components = components;
	bind();
	glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(float), data, GL_STATIC_DRAW);
	unbind();
}

vertex_buffer::~vertex_buffer()
{ 
	const GLuint buf_id = id();
	if(buf_id) {
		glDeleteBuffers(1, &buf_id);
	}
}

void vertex_buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id());
}

void vertex_buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const vertex_buffer& vertex_buffer::operator=(const vertex_buffer& vbo)
{
	id(vbo.id()); 
	m_components = vbo.components();
	m_size = vbo.size();
	return *this;
}