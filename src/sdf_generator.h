#pragma once
#include "color.h"
#include "image.h"

namespace sdfgen {

	class sdf_generator {
	private:
		uint32_t m_color;
		float m_spread;
		uint32_t m_downscale;

	public:
		static constexpr uint32_t default_color = 0xFFFFFFFF;
		static constexpr float default_spread = 32;
		static constexpr int32_t default_downscale = 1;

		sdf_generator(const uint32_t color = default_color, const float spread = default_spread, const int32_t downscale = default_downscale);
		~sdf_generator();

		/** @see #set_color(Color) */
		uint32_t get_color() const { return m_color; }

		/**
		* Sets the color to be used for the output image. Its alpha component is ignored.
		* Defaults to white, which is convenient for multiplying by a color value at runtime.
		*/
		color set_color(const uint32_t color) { const uint32_t old = m_color; m_color = color; return old; }

		/** @see #setDownscale(int) */
		int32_t set_downscale() { return m_downscale; }

		/**
		* Sets the factor by which to downscale the image during processing.
		* The output image will be smaller than the input image by this factor, rounded downwards.
		*
		* <p> For greater accuracy, images to be used as input for a distance field are often
		* generated at higher resolution.
		*
		* @param downscale a positive integer
		* @throws IllegalArgumentException if downscale is not positive
		*/
		int32_t set_downscale(const int32_t downscale) { const int32_t old = downscale; m_downscale = downscale; return old; }

		/** @see #set_spread(float) */
		float get_spread() const { return m_spread; }

		/**
		* Sets the spread of the distance field. The spread is the maximum distance in pixels
		* that we'll scan while for a nearby edge. The resulting distance is also normalized
		* by the spread.
		*
		* @param spread a positive number
		* @throws IllegalArgumentException if spread is not positive
		*/
		float set_spread(const float spread) { const float old = spread; m_spread = spread; return old; }

		/**
		* Process the image into a distance field.
		*
		* The input image should be binary (black/white), but if not, see {@link #isInside(int)}.
		*
		* The returned image is a factor of {@code upscale} smaller than {@code inImage}.
		* Opaque pixels more than {@link #spread} away in the output image from white remain opaque;
		* transparent pixels more than {@link #spread} away in the output image from black remain transparent.
		* In between, we get a smooth transition from opaque to transparent, with an alpha value of 0.5
		* when we are exactly on the edge.
		*
		* @param inImage the image to process.
		* @return the distance field image
		*/
		image_ptr generate(const image_ptr& input_image) { return generate(*input_image); }
		image_ptr generate(const image& input_image);

	private:
		/**
		* Returns {@code true} if the color is considered as the "inside" of the image,
		* {@code false} if considered "outside".
		*
		* <p> Any color with one of its color channels at least 128
		* <em>and</em> its alpha channel at least 128 is considered "inside".
		*/
		inline static bool is_inside(const uint32_t color) { return (color & 0x808080) != 0 && (color & 0x80000000) != 0; }

		/**
		* For a distance as returned by {@link #findSignedDistance}, returns the corresponding "RGB" (really RGBA) color value.
		*
		* @param signedDistance the signed distance of a pixel
		* @return an RGBA color value suitable for {@link BufferedImage#setRGB}.
		*/
		uint32_t distance_to_rgb(const float signed_distance);

		/**
		* Caclulate the squared distance between two points
		*
		* @param x1 The x coordinate of the first point
		* @param y1 The y coordiante of the first point
		* @param x2 The x coordinate of the second point
		* @param y2 The y coordinate of the second point
		* @return The squared distance between the two points
		*/
		inline static int square_distance(const int x1, const int y1, const int x2, const int y2)
		{
			const int dx = x1 - x2;
			const int dy = y1 - y2;
			return dx*dx + dy*dy;
		}

		/**
		* Returns the signed distance for a given point.
		*
		* For points "inside", this is the distance to the closest "outside" pixel.
		* For points "outside", this is the <em>negative</em> distance to the closest "inside" pixel.
		* If no pixel of different color is found within a radius of {@code spread}, returns
		* the {@code -spread} or {@code spread}, respectively.
		*
		* @param centerX the x coordinate of the center point
		* @param centerY the y coordinate of the center point
		* @param bitmap the array representation of an image, {@code true} representing "inside"
		* @return the signed distance
		*/
		float find_signed_distance(const int x_center, const int y_center, const bool * bitmap, const int width, const int height);
	};

	__declspec(dllexport) bool sdf_generate_export(
		const uint8_t * input_buffer, const uint32_t input_width, const uint32_t input_height,
		const int32_t downscale, const float spread,
		uint8_t * output_buffer);
}