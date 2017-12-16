#pragma once
#include <stdint.h>
#include <vector>
#include "opengl_object.h"
#include "tools.h"

namespace sdfgen {

	// This will be GL_STATIC_DRAW only.
	class vertex_buffer : public opengl_object {
	private:
		unsigned int m_components;
		size_t m_size;

	public:
		vertex_buffer(const float * data, const size_t size, const unsigned int components);
		virtual ~vertex_buffer();

		unsigned int components() const { return m_components; }
		size_t size() const { return m_size; }

		void bind() const override;
		void unbind() const override;

		const vertex_buffer& operator=(const vertex_buffer& vbo);
		bool operator==(const vertex_buffer& vbo) const { return vbo.id() == id(); }
		bool operator!=(const vertex_buffer& vbo) const { return vbo.id() != id(); }
	};

	typedef std::shared_ptr<vertex_buffer> vbo_ptr;
}
