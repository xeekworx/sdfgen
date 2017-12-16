#include "sdf_generator.h"
#include <algorithm>

#undef min
#undef max

using namespace sdfgen;

sdf_generator::sdf_generator(const uint32_t color, const float spread, const int32_t downscale)
	: m_color(color), m_spread(spread), m_downscale(downscale)
{

}

sdf_generator::~sdf_generator()
{

}

image_ptr sdf_generator::generate(const image& input_image)
{
	const uint32_t in_width = input_image.width();
	const uint32_t in_height = input_image.height();
	const uint32_t out_width = in_width / m_downscale;
	const uint32_t out_height = in_height / m_downscale;
	const uint32_t bits = input_image.bits();
	bool * bitmap = new bool[in_width * in_height];
	image_ptr out_image = std::make_shared<sdfgen::image>(out_width, out_height);
	float signed_distance = 0.0f;
	const uint32_t * in_pixels = (const uint32_t*) input_image.pixels();
	uint32_t * out_pixels = (uint32_t*) out_image->pixels();
	

	for(uint32_t y = 0; y < in_height; ++y) {
		for(uint32_t x = 0; x < in_width; ++x) {
			bitmap[y * in_width + x] = is_inside(in_pixels[y * in_width + x]);
		}
	}
	
	for(uint32_t y = 0; y < out_height; ++y) {
		for(uint32_t x = 0; x < out_width; ++x) {
			signed_distance = find_signed_distance(
				(x * m_downscale) + (m_downscale / 2),
				(y * m_downscale) + (m_downscale / 2),
				bitmap, 
				in_width, 
				in_height
			);
			out_pixels[y * out_width + x] = distance_to_rgb(signed_distance);
		}
	}

	delete[] bitmap;
	
	return out_image;
}

uint32_t sdf_generator::distance_to_rgb(const float signed_distance)
{
	float alpha = 0.5f + 0.5f * (signed_distance / m_spread);
	alpha = std::min<float>(1.0f, std::max<float>(0.0f, alpha)); // compensate for rounding errors
	uint8_t alpha_byte = (uint8_t) (alpha * 255.0f);
	return (alpha_byte << 24) | (m_color & 0xFFFFFF);
}

float sdf_generator::find_signed_distance(const int center_x, const int center_y, const bool * bitmap, const int width, const int height)
{
	bool base = bitmap[center_y * width + center_x];

	int delta = (int) ceil(m_spread);
	int start_x = std::max<int>(0, center_x - delta);
	int end_x = std::min<int>(width - 1, center_x + delta);
	int start_y = std::max<int>(0, center_y - delta);
	int end_y = std::min<int>(height - 1, center_y + delta);
	int sqrt_distance = 0;
	int closest_sqrt_distance = delta * delta;

	for(int y = start_y; y <= end_y; ++y) {
		for(int x = start_x; x <= end_x; ++x) {
			if(base != bitmap[y * width + x]) {
				sqrt_distance = square_distance(center_x, center_y, x, y);
				if(sqrt_distance < closest_sqrt_distance) {
					closest_sqrt_distance = sqrt_distance;
				}
			}
		}
	}

	float closest_distance = (float) sqrt(closest_sqrt_distance);
	return (base ? 1 : -1) * std::min<float>(closest_distance, m_spread);
}

bool sdfgen::sdf_generate_export(
	const uint8_t * input_buffer, const uint32_t input_width, const uint32_t input_height,
	const int32_t downscale, const float spread,
	uint8_t * output_buffer)
{
	// CHECKS:
	if(!input_buffer || !output_buffer) return false;

	// CREATE INPUT IMAGE:
	sdfgen::image_ptr in_image = std::make_shared<sdfgen::image>((sdfgen::byte_ptr) input_buffer, input_width, input_height);

	// GENERATE SIGNED DISTANCE FIELD:
	sdf_generator gen(sdf_generator::default_color, spread, downscale);
	sdfgen::image_ptr out_image = gen.generate(in_image);
	if(out_image == nullptr) return false;

	// COPY OUTPUT IMAGE INTO OUTPUT BUFFER:
	std::memcpy(output_buffer, out_image->pixels(), out_image->length());

	// SUCCESS:
	return true;
}