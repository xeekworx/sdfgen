#include "color.h"

using namespace sdfgen;

const color color::transparent(0xFFFFFF00);
const color color::white(0xFFFFFFFF);
const color color::black(0x000000FF);
const color color::red(0xFF0000FF);
const color color::green(0x00FF00FF);
const color color::blue(0x0000FFFF);

color::color(const color& c)
{
	m_data = c.m_data;
}

color::color(const uint32_t hex_code)
{
	hex(hex_code);
}

color::color(const glm::vec4& vec)
{
	m_data = vec;
}

color::color(const byte r, const byte g, const byte b, const byte a)
{
	set(r, g, b, a);
}

color::color(const float r, const float g, const float b, const float a)
{
	set(r, g, b, a);
}

color::~color()
{

}

void color::set(const byte r, const byte g, const byte b, const byte a)
{
	m_data[0] = (float) r / 255.0f;
	m_data[1] = (float) g / 255.0f;
	m_data[2] = (float) b / 255.0f;
	m_data[3] = (float) a / 255.0f;
}

void color::set(const float r, const float g, const float b, const float a)
{
	m_data[0] = r;
	m_data[1] = g;
	m_data[2] = b;
	m_data[3] = a;
}

uint32_t color::hex() const 
{ 
	const byte r = (byte) (m_data[0] * 255.0f);
	const byte g = (byte) (m_data[1] * 255.0f);
	const byte b = (byte) (m_data[2] * 255.0f);
	const byte a = (byte) (m_data[3] * 255.0f);

	return (r << 24) | (g << 16) | (b << 8) | a; 
}

uint32_t color::hex_argb() const
{
	const byte r = (byte) (m_data[0] * 255.0f);
	const byte g = (byte) (m_data[1] * 255.0f);
	const byte b = (byte) (m_data[2] * 255.0f);
	const byte a = (byte) (m_data[3] * 255.0f);

	return (a << 24) | (r << 16) | (g << 8) | b;
}

const color& color::hex(uint32_t hex_code)
{
	m_data[0] = (float) ((hex_code >> 24) & 0xFF) / 255.0f;
	m_data[1] = (float) ((hex_code >> 16) & 0xFF) / 255.0f;
	m_data[2] = (float) ((hex_code >> 8) & 0xFF) / 255.0f;
	m_data[3] = (float) ((hex_code) & 0xFF) / 255.0f;
	return *this;
}

color& color::operator=(const color& c)
{
	m_data = c.m_data;
	return *this;
}

color& color::operator=(const glm::vec4& vec)
{
	m_data = vec;
	return *this;
}

color& color::operator=(const glm::vec3& vec)
{
	m_data[0] = vec[0];
	m_data[1] = vec[1];
	m_data[2] = vec[2];
	m_data[3] = 1.0f;
	return *this;
}

color& color::operator=(const uint32_t hex_code)
{
	hex(hex_code);
	return *this;
}

bool color::operator==(const glm::vec3& vec) const
{
	if(m_data[0] == vec[0] && m_data[1] == vec[1] && m_data[2] == vec[2]) return true;
	else return false;
}