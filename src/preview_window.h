#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "image.h"
#include "texture.h"
#include "vertex_array.h"
#include "sdf_shader.h"
#include "basic_shader.h"
#include "color.h"

namespace sdfgen {
	
	class preview_window final {
	private:
		static bool s_initialized;
		static bool s_glad_initialized;
		bool m_auto_show = false;
		GLFWwindow * m_window;
		sdfgen::color m_background_color;
		sdfgen::texture_ptr m_texture;
		sdfgen::shader_program_ptr m_shader;
		sdfgen::vao_ptr m_vao;

	public:
		static constexpr uint32_t GL_CONTEXT_VERSION_MAJOR = 4;
		static constexpr uint32_t GL_CONTEXT_VERSION_MINOR = 1;

		enum shader_type : uint32_t { basic_shader, sdf_shader };

		preview_window(const bool auto_show = true);
		~preview_window();

		static bool is_initialized();
		static void initialize();
		static void terminate();
		static void poll_events();

		void create(
			const std::string& title, const sdfgen::image& image,
			const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file,
			const sdfgen::color& color = color::white, const sdfgen::color& background_color = color::black
		);

		void create(
			const std::string& title, const sdfgen::image& image, const float scale,
			const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file, 
			const sdfgen::color& color = color::white, const sdfgen::color& background_color = color::black
		);

		void create(
			const std::string& title, const sdfgen::image& image, const int32_t width, const int32_t height,
			const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file,
			const sdfgen::color& color = color::white, const sdfgen::color& background_color = color::black
		);

		bool is_valid() const { return m_window != NULL; }
		void make_current() const;
		void destroy();
		void show();
		void hide();
		void update();
		void render() const;
		void render(const sdfgen::color& back_color, const bool clear = true) const;
		sdfgen::image_ptr screenshot(const bool use_fbo = true) const;

	private:
		static GLFWwindow * create_glfw_window(const int width, const int height, const std::string& title, const bool show = true);
		static GLFWwindow * create_glfw_window(const int width, const int height, const std::string& title, const bool show, std::string& error_str);
		static shader_program_ptr create_sdf_shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
		static shader_program_ptr create_basic_shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
		static vao_ptr create_vao(const color& color);
	};

}
