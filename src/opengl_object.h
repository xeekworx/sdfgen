#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <stdint.h>

namespace sdfgen {

	enum class opengl_object_type {
		SHADER, SHADER_PROGRAM, TEXTURE2D, VBO, VAO
	};

	class opengl_object {
	private:
		std::string m_name;
		uint32_t m_id;
		opengl_object_type m_type;

	public:
		opengl_object(const opengl_object_type t) : m_id(0U), m_type(t) {}
		opengl_object(const std::string& name, const uint32_t id, const opengl_object_type t) : m_name(name), m_id(id), m_type(t) {}
		virtual ~opengl_object() {}

		const uint32_t id() const { return m_id; }
		const opengl_object_type& type() const { return m_type; }
		const std::string& name() const { return m_name; }
		std::string get_lasterror() const { return lasterror.str(); }

		virtual void bind() const {};
		virtual void unbind() const {};

	protected:
		std::stringstream lasterror;

		const uint32_t& id(const uint32_t i) { m_id = i; return m_id; }

	};

	typedef std::shared_ptr<opengl_object> opengl_object_ptr;
}