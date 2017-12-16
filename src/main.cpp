// Distance-Field Generator.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "image.h"
#include "sdf_generator.h"
#include "preview_window.h"

namespace sdfgen {

	struct args_t {
		bool verbose = true;
		bool preview = false;
		std::string input_file;
		std::string output_sdf_file;
		std::string output_render_file;
		float spread = 32.0f; // 32 for best results
		int downscale = 4; // 4 for best results
	} args;

	using clock = std::chrono::high_resolution_clock;
}

int main(int argc, char *argv[])
{
	// ------------------------------------------------------------------------
	// HANDLE COMMAND-LINE ARGUMENTS:
	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			if(argv[i][0] == '-') {
				if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
					sdfgen::args.verbose = true;
				}
				else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--preview") == 0) {
					sdfgen::args.preview = true;
				}
				else if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--outrender") == 0) {
					if(argc > i + 1) {
						sdfgen::args.output_render_file = argv[++i];
					}
				}
				else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--outsdf") == 0) {
					if(argc > i + 1) {
						sdfgen::args.output_sdf_file = argv[++i];
					}
				}
				else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--spread") == 0) {
					if(argc > i + 1) {
						sdfgen::args.spread = (float) atof(argv[++i]);
					}
				}
				else if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--downscale") == 0) {
					if(argc > i + 1) {
						sdfgen::args.downscale = atoi(argv[++i]);
					}
				}
			}
			else {
				sdfgen::args.input_file = argv[i];
			}
		}
		// Make sure at least a input image was given as an argument:
		if(sdfgen::args.input_file.empty()) {
			std::cout << "At least the input file argument is required" << std::endl;
			return -1;
		}
	}
	else {
		std::cout << "Not enough parameters" << std::endl;
		return -1;
	}

	// ------------------------------------------------------------------------
	// CHECK & OPEN IMAGE:

	if(sdfgen::args.verbose) std::cout << "Loading source image \"" << sdfgen::args.input_file << "\" ..." << std::endl;
	sdfgen::image_ptr source_image;
	try {
		source_image = std::make_shared<sdfgen::image>(sdfgen::args.input_file);
	}
	catch(std::exception e) {
		std::cerr << "Failed to open source image: " << e.what() << std::endl;
		return -1;
	}

	// ------------------------------------------------------------------------
	// SIGNED DISTANCE FIELD GENERATION:

	if(sdfgen::args.verbose) {
		std::cout 
			<< "Generating Signed Distance Field (spread = " 
			<< std::setprecision(2) << std::hex << sdfgen::args.spread 
			<< std::dec << ", downscale = " << sdfgen::args.downscale << ")" 
			<< std::endl;
		std::cout << "Please wait ...";
	}

	auto sdfgen_t1 = sdfgen::clock::now();

	sdfgen::image_ptr output_image = nullptr;
	try {
		sdfgen::sdf_generator gen(0x00000000, sdfgen::args.spread, sdfgen::args.downscale);
		output_image = gen.generate(source_image);
	}
	catch(std::exception e) {
		std::cerr << std::endl << "Failed to generate signed distance field: " << e.what() << std::endl;
		return -1;
	}

	//diewald::DRA dra;
	//dra.init(source_image, 6, 0xFF000000);
	//dra.createDistanceMap();
	//output_image = dra.getImage();

	auto sdfgen_t2 = sdfgen::clock::now();
	std::cout << " [" << std::chrono::duration_cast<std::chrono::milliseconds>(sdfgen_t2 - sdfgen_t1).count() << " ms]" << std::endl;

	// ------------------------------------------------------------------------
	// SAVE SDF OUTPUT IMAGE TO FILE:
	if(!sdfgen::args.output_sdf_file.empty()) {
		if(sdfgen::args.verbose) std::cout << "Saving output to \"" << sdfgen::args.output_sdf_file << "\" ..." << std::endl;
		try {
			output_image->save(sdfgen::args.output_sdf_file);
		}
		catch(std::exception e) {
			std::cerr << "Failed to write image: " << e.what() << std::endl;
			return -1;
		}
	}

	// ------------------------------------------------------------------------
	// DISPLAY STARTUP:

	if(sdfgen::args.verbose) std::cout << "Starting up OpenGL ..." << std::endl;

	sdfgen::preview_window sdf_preview(false), out_preview(false);
	try {
		sdfgen::preview_window::initialize();
		sdf_preview.create("SDF Render Preview", *output_image, 1024, 1024, sdfgen::preview_window::sdf_shader, "../shaders/basic_vertex.glsl", "../shaders/sdf_fragment.glsl", 0xFFFFFFFF, 0x333333FF);
		out_preview.create("SDF Output Preview", *output_image, sdfgen::preview_window::basic_shader, "../shaders/basic_vertex.glsl", "../shaders/basic_fragment.glsl", 0xFFFFFFFF, 0x333333FF);
	}
	catch(std::exception e) {
		std::cerr << "Failed to create SDF Preview Window: " << e.what() << std::endl;
		sdfgen::preview_window::terminate();
		return -1;
	}

	// Version Info:
	if(sdfgen::args.verbose) {
		printf("OpenGL Version: %d.%d\n", GLVersion.major, GLVersion.minor);
		printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
		printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	}

	// ------------------------------------------------------------------------
	// SCREENSHOT:

	try {
		sdfgen::image_ptr screenshot_image = sdf_preview.screenshot();
		screenshot_image->save(sdfgen::args.output_render_file, sdfgen::image::file_format::png);
	}
	catch(std::exception e) {
		std::cerr << "Failed to save screenshot of SDF preview: " << e.what() << std::endl;
		sdfgen::preview_window::terminate();
		return -1;
	}

	// Are we done?
	if(!sdfgen::args.preview) {
		if(sdfgen::args.verbose) std::cout << "Finished." << std::endl;
		return 0;
	}
	else {
		sdf_preview.show();
		out_preview.show();
	}

	// ------------------------------------------------------------------------
	// MAIN LOOP:

	if(sdfgen::args.verbose) std::cout << "Starting Rendering Loop ..." << std::endl;

	// Loop until the user closes the window
	while(sdf_preview.is_valid() && out_preview.is_valid()) {
		sdfgen::preview_window::poll_events();

		sdf_preview.update();
		out_preview.update();
	}

	sdfgen::preview_window::terminate();
	if(sdfgen::args.verbose) std::cout << "Finished." << std::endl;
	return 0;
}
