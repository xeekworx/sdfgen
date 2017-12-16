#pragma once
#include "shader_program.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace sdfgen {

	class sdf_shader : public shader_program {
	private:
		int32_t uniform_obj_width = 0,
			uniform_edge_width = 0,
			uniform_enable_outline = 0,
			uniform_outline_width = 0,
			uniform_outline_edge = 0,
			uniform_outline_offset = 0,
			uniform_outline_color = 0,
			uniform_enable_smart_edge = 0,
			uniform_smart_edge_color = 0,
			uniform_smart_edge_width = 0,
			uniform_smart_edge_dist = 0;

	public:
		struct options {
			float obj_width;
			float edge_width;

			bool enable_outline;
			float outline_width;
			float outline_edge;
			glm::vec4 outline_color;
			glm::vec2 shadow_offset;

			bool enable_smart_edge;
			glm::vec4 smart_edge_color;
			float smart_edge_width;
			float smart_edge_dist;
		};

		sdf_shader(const sdf_shader::options& options, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations);
		virtual ~sdf_shader();

		void set_options(const sdf_shader::options& options) const;
	};

	typedef std::shared_ptr<sdf_shader> sdf_shader_ptr;
}
