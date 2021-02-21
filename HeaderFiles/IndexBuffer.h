#pragma once

#include <GL/glew.h>
#include "ErrorManager.h"

namespace avt {

	class IndexBuffer {
	private:
		GLuint _iboID;
		unsigned int _count;
	public:
		IndexBuffer(const void* data, GLuint count) : _iboID(0), _count(count) {
			glGenBuffers(1, &_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Index Buffer.");
#endif
		}

		~IndexBuffer() {
			glDeleteBuffers(1, &_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not destroy Index Buffer.");
#endif
		}

		void bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
		}

		void unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		GLuint count() const {
			return _count;
		}
	};

}