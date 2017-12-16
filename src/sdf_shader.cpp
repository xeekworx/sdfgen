#include "sdf_shader.h"

using namespace sdfgen;

sdf_shader::sdf_shader(const sdf_shader::options& options, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations)
	: shader_program("Distance Field Shader", shaders, attrib_locations)
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
	uniform_obj_width = glGetUniformLocation(id(), "obj_width");
	uniform_edge_width = glGetUniformLocation(id(), "edge_width");
	uniform_enable_outline = glGetUniformLocation(id(), "enable_outline");
	uniform_outline_width = glGetUniformLocation(id(), "outline_width");
	uniform_outline_edge = glGetUniformLocation(id(), "outline_edge");
	uniform_outline_offset = glGetUniformLocation(id(), "outline_offset");
	uniform_outline_color = glGetUniformLocation(id(), "outline_color");
	uniform_enable_smart_edge = glGetUniformLocation(id(), "enable_smart_edge");
	uniform_smart_edge_color = glGetUniformLocation(id(), "smart_edge_color");
	uniform_smart_edge_width = glGetUniformLocation(id(), "smart_edge_width");
	uniform_smart_edge_dist = glGetUniformLocation(id(), "smart_edge_dist");
	this->unbind();

	// SET UNIFORM VARIABLE VALUES:
	set_options(options);
}

sdf_shader::~sdf_shader()
{

}

void sdf_shader::set_options(const sdf_shader::options& options) const
{
	this->bind();

	if(uniform_obj_width != -1) glUniform1f(uniform_obj_width, options.obj_width);
	if(uniform_edge_width != -1) glUniform1f(uniform_edge_width, options.edge_width);
	if(uniform_enable_outline != -1) glUniform1i(uniform_enable_outline, (GLint) options.enable_outline);
	if(uniform_outline_width != -1) glUniform1f(uniform_outline_width, options.outline_width);
	if(uniform_outline_edge != -1) glUniform1f(uniform_outline_edge, options.outline_edge);
	if(uniform_outline_offset != -1) glUniform2f(uniform_outline_offset, options.shadow_offset[0], options.shadow_offset[1]);
	if(uniform_outline_color != -1) glUniform4f(uniform_outline_color, options.outline_color[0], options.outline_color[1], options.outline_color[2], options.outline_color[3]);
	if(uniform_enable_smart_edge != -1) glUniform1i(uniform_enable_smart_edge, options.enable_smart_edge);
	if(uniform_smart_edge_color != -1) glUniform4f(uniform_smart_edge_color, options.smart_edge_color[0], options.smart_edge_color[1], options.smart_edge_color[2], options.smart_edge_color[3]);
	if(uniform_smart_edge_width != -1) glUniform1f(uniform_smart_edge_width, options.smart_edge_width);
	if(uniform_smart_edge_dist != -1) glUniform1f(uniform_smart_edge_dist, options.smart_edge_dist);

	this->unbind();
}