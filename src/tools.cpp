#include <stdlib.h>
#include <glad/glad.h>
#include "tools.h"

using namespace sdfgen;

std::string sdfgen::glerror_to_string(const GLenum error)
{
	std::string errstr;
	switch(error) {
	case GL_NO_ERROR:
		errstr = "No error has been recorded";
		break;
	case GL_INVALID_ENUM:
		errstr = "An unacceptable value is specified for an enumerated argument";
		break;
	case GL_INVALID_VALUE:
		errstr = "A numeric argument is out of range";
		break;
	case GL_INVALID_OPERATION:
		errstr = "The specified operation is not allowed in the current state";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errstr = "The framebuffer object is not complete";
		break;
	case GL_OUT_OF_MEMORY:
		errstr = "There is not enough memory left to execute the command";
		break;
	case GL_STACK_UNDERFLOW:
		errstr = "An attempt has been made to perform an operation that would cause an internal stack to underflow";
		break;
	case GL_STACK_OVERFLOW:
		errstr = "An attempt has been made to perform an operation that would cause an internal stack to overflow";
		break;
	default:
		errstr = "Unknown OpenGL error";
		break;
	}

	return errstr;
}
