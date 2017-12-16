#include "basic_shader.h"

using namespace sdfgen;

basic_shader::basic_shader(const basic_shader::options& options, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations)
	: shader_program("Basic Shader", shaders, attrib_locations)
{
	// VALIDATE PARAMETERS:
	// Not enough shaders?
	unsigned vertex_shader_count = 0;
	unsigned frag_shader_count = 0;
	for(auto i = shaders.begin(); i != shaders.end(); ++i) {
		if((*i)->shader_type() == shader::type::VERTEX) vertex_shader_count++;
		if((*i)->shader_type() == shader::type::FRAGMENT) frag_shader_count++;
	}
	if(vertex_shader_count != 1 && frag_shader_count != 1) {
		throw std::exception("Distance Field Shader requires one vertex shader and one fragment shader");
	}

	// GET UNIFORM VARIABLE LOCATIONS:
	this->bind();
	uniform_modulate = glGetUniformLocation(id(), "modulate");
	this->unbind();

	// SET UNIFORM VARIABLE VALUES:
	set_options(options);
}

basic_shader::~basic_shader()
{

}

void basic_shader::set_options(const basic_shader::options& options) const
{
	this->bind();
	if(uniform_modulate != -1) glUniform1i(uniform_modulate, options.modulate);
	this->unbind();
}