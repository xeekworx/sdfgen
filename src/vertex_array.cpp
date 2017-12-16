#include <algorithm>
#include <iterator>
#include "vertex_array.h"

using namespace sdfgen;

vertex_array::vertex_array(const std::vector<vbo_ptr>& vbos) : opengl_object(opengl_object_type::VAO)
{
	GLuint vao_id = 0;
	glGenVertexArrays(1, &vao_id);

	if(vao_id) {
		m_vbos.assign(vbos.begin(), vbos.end());
		id(vao_id);
		bind();

		GLuint index = 0;
		for(auto ivbo = m_vbos.begin(); ivbo != m_vbos.end(); ++ivbo, ++index) {
			(*ivbo)->bind(); // Bind Vertex Buffer Object
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, (*ivbo)->components(), GL_FLOAT, GL_FALSE, 0, NULL);
		}

		unbind();
	}
	else {
		throw std::exception(glerror_to_string(glGetError()).c_str());
	}
}

vertex_array::~vertex_array()
{
	const GLuint vao_id = id();
	if(vao_id) {
		glDeleteVertexArrays(1, &vao_id);
		unbind();
	}
}

void vertex_array::enable_attrib_array(unsigned int index) const
{
	if(index < m_vbos.size()) {
		bind();
		glEnableVertexAttribArray(index);
		unbind();
	}
}

void vertex_array::disable_attrib_array(unsigned int index) const
{
	if(index < m_vbos.size()) {
		bind();
		glDisableVertexAttribArray(index);
		unbind();
	}
}

void vertex_array::bind() const
{
	glBindVertexArray(id());
}

void vertex_array::unbind() const
{
	glBindVertexArray(0);
}