#pragma once
#include "shader_program.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace sdfgen {

	class basic_shader : public shader_program {
	private:
		int32_t uniform_modulate = 0;

	public:
		struct options {
			bool modulate;
		};

		basic_shader(const basic_shader::options& options, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations);
		virtual ~basic_shader();

		void set_options(const basic_shader::options& options) const;
	};

	typedef std::shared_ptr<basic_shader> basic_shader_ptr;
}
