#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <glad/glad.h>

#ifdef _WIN32
#	include <minwindef.h>
#	define SDFGEN_MAX_PATH MAX_PATH
#elif __APPLE__
#	define SDFGEN_MAX_PATH MAXPATHLEN
#else
#	define SDFGEN_MAX_PATH PATHMAX
#endif

namespace sdfgen {

	typedef uint8_t byte;
	typedef byte * byte_ptr;

	std::string glerror_to_string(const GLenum error);

}