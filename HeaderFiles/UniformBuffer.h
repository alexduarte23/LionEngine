#pragma once

#include <GL/glew.h>
#include <initializer_list>
#include "Mat4.h"
#include "ErrorManager.h"

namespace avt{

	class UniformBuffer {
	private:
		GLuint _uboID;
		GLuint _bindingPoint;
		GLsizei _size;
	public:
		UniformBuffer(GLsizeiptr size, GLuint bindingPoint) : _uboID(0), _size((GLsizei)size) {
			glGenBuffers(1, &_uboID);
			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboID);
		
			_bindingPoint = bindingPoint;

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

		void upload(const void* data, GLsizeiptr size) {
			if (!data || size <= 0) return;
			if (size > _size) {
				std::cerr << "Uniform Buffer data upload FAIL: data size larger than buffer size." << std::endl;
				return;
			}

			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not upload data to Uniform Buffer.");
#endif
		}

		template<typename T>
		void upload(const std::vector<T>& uData) {
			upload(uData.data(), uData.size() * sizeof(T));
		}

		void upload(std::initializer_list<Mat4> mList) {
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

		void setBindingPoint(GLuint bindingPoint) {
			glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboID);
			_bindingPoint = bindingPoint;

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not set Uniform Buffer binding point.");
#endif
		}

		GLuint bindingPoint() const {
			return _bindingPoint;
		}

		GLsizei size() const {
			return _size;
		}
	};

}