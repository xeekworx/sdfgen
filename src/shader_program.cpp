#include "shader_program.h"
#include "glad\glad.h"
#include "tools.h"

using namespace sdfgen;

static bool program_error(const GLuint id, const GLuint flag, std::string& error_out);
static bool program_error(const GLuint id, const GLuint flag);

shader_program::shader_program(const std::string& name, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations)
	: opengl_object(name, 0U, opengl_object_type::SHADER_PROGRAM)
{
	if(shaders.empty()) throw std::exception("Program's shader list is empty");
	else {
		std::string status_out;

		m_shaders.assign(shaders.begin(), shaders.end());

		// CREATE SHADER PROGRAM:
		if(0 == id(glCreateProgram())) {
			throw std::exception("Failed to create shader program");
		}

		// ATTACH SHADERS:
		for(auto s = m_shaders.begin(); s != m_shaders.end(); ++s) {
			if((*s)) glAttachShader(id(), (*s)->id());
			else {
				throw std::exception("Null shader encountered when creating shader program");
			}
		}

		// BIND VERTEX ATTRIBUTE LOCATIONS:
		for(auto i = attrib_locations.begin(); i != attrib_locations.end(); ++i) {
			glBindAttribLocation(id(), (*i).index, (*i).name.c_str());
		}

		// LINK SHADER PROGRAM:
		glLinkProgram(id());
		if(program_error(id(), GL_LINK_STATUS, status_out)) {
			throw std::exception((std::string("Shader program link failed:\n") + status_out).c_str());
		}

		// VALIDATE SHADER PROGRAM:
		glValidateProgram(id());
		if(program_error(id(), GL_VALIDATE_STATUS, status_out)) {
			throw std::exception((std::string("Shader program validation failed:\n") + status_out).c_str());
		}
	}
}

shader_program::~shader_program()
{
	const GLuint program_id = id();
	if(program_id) {
		for(auto s = m_shaders.begin(); s != m_shaders.end(); ++s) {
			glDetachShader(program_id, (*s)->id());
		}
		glDeleteProgram(program_id);
		unbind();
	}
}

void shader_program::bind() const
{
	glUseProgram(id());
}

void shader_program::unbind() const
{
	glUseProgram(0);
}

bool program_error(const GLuint id, const GLuint flag)
{
	std::string error_out;
	return program_error(id, flag, error_out);
}

bool program_error(const GLuint id, const GLuint flag, std::string& error_out)
{
	GLint result = 0;
	glGetProgramiv(id, flag, &result);

	if(result == GL_FALSE) {
		GLchar error[1024] = {};
		glGetProgramInfoLog(id, sizeof(error), NULL, error);
		error_out = error;
	}

	return (result == GL_FALSE);
}