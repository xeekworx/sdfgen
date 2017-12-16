#include <fstream>
#include "shader.h"
#include "glad\glad.h"
#include "tools.h"

using namespace sdfgen;

static GLenum to_gl_shader_type(const shader::type shader_type);
static bool shader_error(const GLuint id, const GLuint flag, std::string& error_out);
static bool shader_error(const GLuint id, const GLuint flag);

shader::shader(const std::string& name, const std::string& sourcecode, const shader::type shader_type) 
	: opengl_object(name, 0U, opengl_object_type::SHADER)
{
	if(sourcecode.empty()) throw std::exception("Shader sourcecode is empty");
	else {
		m_sourcecode = sourcecode;
		m_shader_type = shader_type;

		// CREATE SHADER:
		if(0 == id(glCreateShader(to_gl_shader_type(shader_type)))) {
			lasterror << L"Failed to create shader" << std::endl;
			throw std::exception(get_lasterror().c_str());
		}
		
		// COPY SHADER SOURCE:
		const GLchar * source_strings = m_sourcecode.c_str();
		const GLint source_length = (GLint) m_sourcecode.length();
		glShaderSource(id(), 1, &source_strings, &source_length);

		// COMPILE SHADER:
		glCompileShader(id());
		std::string compile_error;
		if(shader_error(id(), GL_COMPILE_STATUS, compile_error)) {
			lasterror << L"Shader compilation failed:\n" << compile_error << std::endl;
			throw std::exception(get_lasterror().c_str());
		}
	}
}

shader::~shader()
{
	const GLuint shader_id = id();
	if(shader_id) {
		glDeleteShader(shader_id);
	}
}

std::string shader::load(const std::string& file)
{
	std::stringstream result;
	std::string line;
	std::ifstream infile;

	infile.open(file);
	if(!infile) {
		throw std::exception("Failed to open shader source file");
	}

	while(!infile.eof()) {
		std::getline(infile, line); // Saves the line in STRING.
		result << line << std::endl;
	}

	infile.close();
	return result.str();
}

GLenum to_gl_shader_type(const shader::type shader_type)
{
	switch(shader_type) {
	case shader::type::COMPUTE: return GL_COMPUTE_SHADER;
	case shader::type::VERTEX: return GL_VERTEX_SHADER;
	case shader::type::TESS_CONTROL: return GL_TESS_CONTROL_SHADER;
	case shader::type::TESS_EVALUATION: return GL_TESS_EVALUATION_SHADER;
	case shader::type::GEOMETRY: return GL_GEOMETRY_SHADER;
	case shader::type::FRAGMENT: return GL_FRAGMENT_SHADER;
	default: return 0;
	}
}

bool shader_error(const GLuint id, const GLuint flag)
{
	std::string error_out;
	return shader_error(id, flag, error_out);
}

bool shader_error(const GLuint id, const GLuint flag, std::string& error_out)
{
	GLint result = 0;
	glGetShaderiv(id, flag, &result);

	if(result == GL_FALSE) {
		GLchar error[1024] = {};
		glGetShaderInfoLog(id, sizeof(error), NULL, error);
		error_out = error;
	}

	return (result == GL_FALSE);
}
