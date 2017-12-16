#pragma once
#include "shader.h"
#include <vector>

namespace sdfgen {

	struct attrib_location {
		unsigned int index;
		std::string name;
	};

	class shader_program : public opengl_object {
	private:
		std::vector<shader_ptr> m_shaders;
		
	public:
		shader_program(const std::string& name, const std::vector<shader_ptr>& shaders, const std::vector<attrib_location>& attrib_locations);
		virtual ~shader_program();

		const std::vector<shader_ptr>& shaders() const { return m_shaders; }

		void bind() const override;
		void unbind() const override;
	};

	typedef std::shared_ptr<shader_program> shader_program_ptr;
}

