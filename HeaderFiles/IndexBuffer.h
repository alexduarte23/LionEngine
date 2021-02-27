#pragma once

#include <GL/glew.h>
#include <vector>
#include "ErrorManager.h"

namespace avt {

	class IndexBuffer {
	private:
		GLuint _iboID;
		unsigned int _count;
		GLsizei _capacityCount;
	public:
		IndexBuffer(const void* data, GLuint count) : _iboID(0), _count(data ? count : 0), _capacityCount(count) {
			glGenBuffers(1, &_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Index Buffer.");
#endif
		}

		IndexBuffer(const std::vector<unsigned int>& indices)
			: IndexBuffer(indices.data(), (GLuint)indices.size()) {}

		IndexBuffer(GLuint count)
			: IndexBuffer(nullptr, count) {}

		~IndexBuffer() {
			glDeleteBuffers(1, &_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not destroy Index Buffer.");
#endif
		}

		void upload(const void* data, GLuint count) {
			if (!data || count <= 0) return;
			if (count > _count) {
				std::cerr << "Index Buffer data upload FAIL: data size larger than buffer size." << std::endl;
				return;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(GLuint), data);
			_count = count;

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not upload data to Index Buffer.");
#endif
		}

		void upload(const std::vector<unsigned int>& indices) {
			upload(indices.data(), (GLuint)indices.size());
		}

		void resize(GLsizeiptr capacityCount, bool keepData = true) {
			if (capacityCount == _capacityCount && keepData || capacityCount <= 0) return;

			if (keepData) {
				if (capacityCount < _count) _count = (GLsizei)capacityCount;
				// create aux buffer
				GLuint auxID;
				glGenBuffers(1, &auxID);
				glBindBuffer(GL_COPY_WRITE_BUFFER, auxID);
				glBufferData(GL_COPY_WRITE_BUFFER, _count * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

				// copy data to aux
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
				glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _count * sizeof(GLuint));

				// re-allocate vb and copy data from aux
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacityCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
				glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, _count * sizeof(GLuint));

				// delete aux buffer
				glDeleteBuffers(1, &auxID);
				glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
			}
			else {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacityCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
				_count = 0;
			}
			_capacityCount = (GLsizei)capacityCount;

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not resize Index Buffer.");
#endif
		}

		void clear() {
			_count = 0;
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

		GLuint capacityCount() const {
			return _capacityCount;
		}

	};

}