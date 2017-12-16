#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "opengl_object.h"
#include "vertex_buffer.h"
#include "shader_program.h"
#include "tools.h"

namespace sdfgen {

	class vertex_array : public opengl_object {
	private:
		std::vector<vbo_ptr> m_vbos;

	public:
		vertex_array(const std::vector<vbo_ptr>& vbos);
		virtual ~vertex_array();

		const std::vector<vbo_ptr>& vbos() const { return m_vbos; }

		void enable_attrib_array(unsigned int index) const;
		void disable_attrib_array(unsigned int index) const;
		void bind() const override;
		void unbind() const override;
	};

	typedef std::shared_ptr<vertex_array> vao_ptr;
}
