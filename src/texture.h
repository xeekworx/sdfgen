#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include "opengl_object.h"
#include "image.h"
#include "tools.h"

namespace sdfgen {

	class texture : public opengl_object {
	private:
		uint32_t m_width = 0;
		uint32_t m_height = 0;

	public:
		texture(const sdfgen::image& image, const bool mipmap = true, const bool anisotropic = true);
		texture(const byte_ptr pixels, const uint32_t width, const uint32_t height, const bool mipmap = true, const bool anisotropic = true);
		virtual ~texture();

		const uint32_t& width() const { return m_width; }
		const uint32_t& height() const { return m_height; }

		void bind() const override;
		void unbind() const override;
	};

	typedef std::shared_ptr<texture> texture_ptr;
}
