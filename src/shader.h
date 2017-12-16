#pragma once
#include "opengl_object.h"

namespace sdfgen {

	class shader : public opengl_object {
	public:
		enum class type { UNKNOWN, COMPUTE, VERTEX, TESS_CONTROL, TESS_EVALUATION, GEOMETRY, FRAGMENT };

	private:
		std::string m_sourcecode;
		type m_shader_type = type::UNKNOWN;

	public:
		shader(const std::string& name, const std::string& sourcecode, const shader::type shader_type);
		virtual ~shader();

		const type& shader_type() const { return m_shader_type; }
		const std::string& source() const { return m_sourcecode; }

		static std::string load(const std::string& file);

	};

	typedef std::shared_ptr<shader> shader_ptr;
}

