#pragma once

#include <GL/glew.h>
#include <initializer_list>
#include "Mat4.h"
#include "ErrorManager.h"

namespace avt{

	class UniformBuffer {
	private:
		GLuint _uboID;
		GLsizei _size;
	public:
		UniformBuffer(GLsizeiptr size, GLuint ubBinding) : _uboID(0), _size((GLsizei)size) {
			glGenBuffers(1, &_uboID);
			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, ubBinding, _uboID);
		
#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Uniform Buffer.");
#endif
		}

		~UniformBuffer() {
			glDeleteBuffers(1, &_uboID);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not destroy Uniform Buffer.");
#endif
		}

		void fill(std::initializer_list<Mat4> mList) {
			if (mList.size() * 16 * sizeof(GLfloat) > _size) return;

			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
			GLintptr pos = 0;
			for (auto& m : mList) {
				glBufferSubData(GL_UNIFORM_BUFFER, pos, 16 * sizeof(GLfloat), m.data());
				pos += 16 * sizeof(GLfloat);
			}

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could upload data to Uniform Buffer.");
#endif
		}

		void bind() const {
			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
		}

		void unbind() const {
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	};

}