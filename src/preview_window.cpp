#include "preview_window.h"

using namespace sdfgen;

bool sdfgen::preview_window::s_initialized = false;
bool sdfgen::preview_window::s_glad_initialized = false;

preview_window::preview_window(const bool auto_show)
	: m_window(nullptr), m_texture(nullptr), m_shader(nullptr), m_vao(nullptr), m_background_color(0x000000FF), m_auto_show(auto_show)
{

}

preview_window::~preview_window()
{
	if(m_window) glfwDestroyWindow(m_window);
}

bool preview_window::is_initialized()
{
	return s_initialized;
}

void preview_window::initialize()
{
	if(!is_initialized()) {
		if(!glfwInit()) {
			throw std::exception("Failed to initialize GLFW");
		}
		else {
			preview_window::s_initialized = true;
		}
	}
}

void preview_window::terminate()
{
	if(is_initialized()) {
		glfwTerminate();
		preview_window::s_initialized = false;
	}
}

void preview_window::poll_events()
{
	if(is_initialized()) {
		glfwPollEvents();
	}
}

void preview_window::create(
	const std::string& title, const sdfgen::image& image,
	const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file,
	const sdfgen::color& color, const sdfgen::color& background_color
)
{
	return create(title, image, image.width(), image.height(), shader_type, vertex_shader_file, frag_shader_file, color, background_color);
}

void preview_window::create(
	const std::string& title, const sdfgen::image& image, const float scale,
	const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file,
	const sdfgen::color& color, const sdfgen::color& background_color
)
{
	return create(title, image, (int32_t)((float) image.width() * scale), (int32_t)((float) image.height() * scale), shader_type, vertex_shader_file, frag_shader_file, color, background_color);
}

void preview_window::create(
	const std::string& title, const sdfgen::image& image, const int32_t width, const int32_t height,
	const uint32_t shader_type, const std::string& vertex_shader_file, const std::string& frag_shader_file,
	const sdfgen::color& color, const sdfgen::color& background_color
)
{
	if(!is_initialized()) {
		throw std::exception("Not initialized when creating preview window");
	}

	if(is_valid()) {
		throw std::exception("Tried to create window that is already created");
	}

	// CREATE GLFW WINDOW:
	std::string error_str;
	if(NULL == (m_window = create_glfw_window(width, height, title, m_auto_show, error_str))) {
		throw std::exception(error_str.c_str());
	}

	// CREATE TEXTURE:
	m_texture = std::make_shared<sdfgen::texture>(image, true, true);

	// CREATE SHADER:
	switch(shader_type) {
	case shader_type::basic_shader: 
		m_shader = preview_window::create_basic_shader(vertex_shader_file, frag_shader_file);
		break;
	case shader_type::sdf_shader:
		m_shader = preview_window::create_sdf_shader(vertex_shader_file, frag_shader_file);
		break;
	}

	// CREATE VERTEX ARRAY OBJECT:
	m_vao = preview_window::create_vao(color);

	m_background_color = background_color;
}

void preview_window::destroy()
{
	if(is_valid()) {
		glfwSetWindowShouldClose(m_window, 1);
		glfwDestroyWindow(m_window);
		m_window = NULL;
	}
}

void preview_window::make_current() const
{
	if(is_valid()) {
		glfwMakeContextCurrent(m_window);
	}
}

GLFWwindow * preview_window::create_glfw_window(const int width, const int height, const std::string& title, const bool show)
{
	std::string error_str;
	return create_glfw_window(width, height, title, show, error_str);
}

GLFWwindow * preview_window::create_glfw_window(const int width, const int height, const std::string& title, const bool show, std::string& error_str)
{
	// Is GLFW initialized?
	if(!is_initialized()) return NULL;

	// Preconfigure OpenGL & Window:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, preview_window::GL_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, preview_window::GL_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_VISIBLE, (int) show);

	// Create GLFWwindow:
	GLFWwindow * new_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!new_window) return NULL;

	// Make the window's context current:
	glfwMakeContextCurrent(new_window);

	// Initialize GLAD:
	if(!preview_window::s_glad_initialized) {
		if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			error_str = "Failed to initialize GLAD";
			glfwDestroyWindow(new_window);
			return NULL;
		}
	}

	// Setup OpenGL:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	return new_window;
}

shader_program_ptr preview_window::create_sdf_shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file)
{
	constexpr unsigned sdf_attrib_count = 3;
	sdfgen::attrib_location sdf_attrib_loc[sdf_attrib_count] = { { 0, "position" }, { 1, "color" }, { 2, "texcoord" } };
	sdfgen::sdf_shader::options sdf_options;

	sdf_options.obj_width = 0.5f;
	sdf_options.edge_width = 0.12f;

	sdf_options.enable_outline = false;
	sdf_options.outline_width = 0.5f;
	sdf_options.outline_edge = 0.3f;
	sdf_options.outline_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	sdf_options.shadow_offset = glm::vec2(-0.004f, -0.005f);

	sdf_options.enable_smart_edge = false;
	sdf_options.smart_edge_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sdf_options.smart_edge_width = 0.1f;
	sdf_options.smart_edge_dist = 0.3f;

	sdfgen::sdf_shader_ptr sdf_shader_program = nullptr;
	std::vector<sdfgen::shader_ptr> shaders;
	shaders.push_back(std::make_shared<sdfgen::shader>("Basic Vertex", sdfgen::shader::load(vertex_shader_file), sdfgen::shader::type::VERTEX));
	shaders.push_back(std::make_shared<sdfgen::shader>("SDF Fragment", sdfgen::shader::load(fragment_shader_file), sdfgen::shader::type::FRAGMENT));
	sdf_shader_program = std::make_shared<sdfgen::sdf_shader>(sdf_options, shaders, std::vector<sdfgen::attrib_location>(sdf_attrib_loc, sdf_attrib_loc + sdf_attrib_count));

	sdf_shader_program->bind();

	return sdf_shader_program;
}

shader_program_ptr preview_window::create_basic_shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file)
{
	constexpr unsigned basic_attrib_count = 3;
	sdfgen::attrib_location basic_attrib_loc[basic_attrib_count] = { { 0, "position" }, { 1, "color" }, { 2, "texcoord" } };
	sdfgen::basic_shader::options basic_options;

	basic_options.modulate = true;

	sdfgen::basic_shader_ptr basic_shader_program = nullptr;
	std::vector<sdfgen::shader_ptr> shaders;
	shaders.push_back(std::make_shared<sdfgen::shader>("Basic Vertex", sdfgen::shader::load(vertex_shader_file), sdfgen::shader::type::VERTEX));
	shaders.push_back(std::make_shared<sdfgen::shader>("Basic Fragment", sdfgen::shader::load(fragment_shader_file), sdfgen::shader::type::FRAGMENT));
	basic_shader_program = std::make_shared<sdfgen::basic_shader>(basic_options, shaders, std::vector<sdfgen::attrib_location>(basic_attrib_loc, basic_attrib_loc + basic_attrib_count));

	basic_shader_program->bind();

	return basic_shader_program;
}

vao_ptr preview_window::create_vao(const color& color)
{
	constexpr float size = 1.0f;
	float points[] = {
		-size,  size, 0,	// bottom left corner
		-size, -size, 0,	// top left corner
		size, -size, 0,		// top right corner
		size,  size, 0		// bottom right corner
	};

	float colors[] = {
		color[0], color[1], color[2], color[3],
		color[0], color[1], color[2], color[3],
		color[0], color[1], color[2], color[3],
		color[0], color[1], color[2], color[3]
	};

	float tex_coords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	sdfgen::vbo_ptr position_vbo = std::make_shared<vertex_buffer>(points, 12, 3);
	sdfgen::vbo_ptr color_vbo = std::make_shared<vertex_buffer>(colors, 16, 4);
	sdfgen::vbo_ptr texture_vbo = std::make_shared<vertex_buffer>(tex_coords, 8, 2);

	std::vector<sdfgen::vbo_ptr> vbo_vector;
	vbo_vector.push_back(position_vbo);
	vbo_vector.push_back(color_vbo);
	vbo_vector.push_back(texture_vbo);

	sdfgen::vao_ptr vao = std::make_shared<sdfgen::vertex_array>(vbo_vector);
	return vao;
}

void preview_window::show()
{
	if(is_valid()) glfwShowWindow(m_window);
}

void preview_window::hide()
{
	if(is_valid()) glfwHideWindow(m_window);
}

void preview_window::update()
{
	if(!is_valid()) return;
	else {
		render();
		glfwSwapBuffers(m_window);

		if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) || glfwWindowShouldClose(m_window)) {
			destroy();
		}
	}
}

void preview_window::render() const
{
	render(m_background_color);
}

void preview_window::render(const sdfgen::color& back_color, const bool clear) const
{
	make_current();

	if(clear) {
		glClearColor(back_color[0], back_color[1], back_color[2], back_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	m_texture->bind();
	m_vao->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

sdfgen::image_ptr preview_window::screenshot(const bool use_fbo) const
{
	make_current();

	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	GLuint FramebufferName = 0, renderedTexture = 0;
	if(use_fbo) {
		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, view[2], view[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	render(0xFFFFFF00, true);

	std::vector<GLubyte> screenshot_pixels(view[2] * 4 * view[3]);
	glReadPixels(0, 0, view[2], view[3], GL_RGBA, GL_UNSIGNED_BYTE, screenshot_pixels.data());

	sdfgen::image_ptr screenshot_image = std::make_shared<sdfgen::image>((sdfgen::byte_ptr) screenshot_pixels.data(), view[2], view[3]);
	screenshot_image->flip(image::flip_type::vertical);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(use_fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(0, &FramebufferName);
		glDeleteTextures(0, &renderedTexture);
	}
	
	return screenshot_image;
}
