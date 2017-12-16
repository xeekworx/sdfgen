#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include "tools.h"

namespace sdfgen {

	class color {
	private:
		glm::vec4 m_data;

	public:
		static const color transparent;
		static const color white;
		static const color black;
		static const color red;
		static const color green;
		static const color blue;

		color(const color& c);
		color(const uint32_t hex_code = 0x00000000);
		color(const glm::vec4& vec);
		color(const byte r, const byte g, const byte b, const byte a = 255);
		color(const float r, const float g, const float b, const float a = 1.0f);
		virtual ~color();

		void set(const byte r, const byte g, const byte b, const byte a = 255);
		void set(const float r, const float g, const float b, const float a = 1.0f);
		const glm::vec4& vector4() const { return m_data; }
		const glm::vec3& vector3() const { return (glm::vec3) m_data; }
		uint32_t hex() const;
		uint32_t hex_argb() const;
		const color& hex(uint32_t hex_code);
		void reset() { hex(0x00000000); }

		const float& r_float() const { return m_data[0]; }
		const float& g_float() const { return m_data[1]; }
		const float& b_float() const { return m_data[2]; }
		const float& a_float() const { return m_data[3]; }

		byte r_byte() const { return (byte) (m_data[0] * 255.0f); }
		byte g_byte() const { return (byte) (m_data[1] * 255.0f); }
		byte b_byte() const { return (byte) (m_data[2] * 255.0f); }
		byte a_byte() const { return (byte) (m_data[3] * 255.0f); }

		operator uint32_t() const { return hex(); }
		operator const glm::vec4&() const { return vector4(); }
		operator const glm::vec3&() const { return vector3(); }
		color& operator=(const color& c);
		color& operator=(const glm::vec4& vec);
		color& operator=(const glm::vec3& vec);
		color& operator=(const uint32_t hex_code);
		bool operator==(const color& c) const { return m_data == c.m_data; }
		bool operator==(const glm::vec4& vec) const { return m_data == vec; }
		bool operator==(const glm::vec3& vec) const;
		bool operator!=(const color& c) const { return !(*this == c); }
		bool operator!=(const glm::vec4& vec) const { return !(*this == vec); }
		bool operator!=(const glm::vec3& vec) const { return !(*this == vec); }
		float& operator[](int i) { return m_data[i]; }
		const float& operator[](int i) const { return m_data[i]; }
	};

}