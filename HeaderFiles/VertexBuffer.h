#pragma once

#include <GL/glew.h>
#include "ErrorManager.h"
#include "VertexBufferLayout.h"

namespace avt{

	class VertexBuffer {
	private:
		GLuint _vboID;
		GLsizei _size;
		VertexBufferLayout _layout;

	public:
		VertexBuffer(const void* data, GLsizeiptr size) : _vboID(0), _size((GLsizei)size) {
			glGenBuffers(1, &_vboID);
			glBindBuffer(GL_ARRAY_BUFFER, _vboID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Vertex Buffer.");
#endif
		}

		VertexBuffer(const void* data, GLsizeiptr size, const VertexBufferLayout& layout) : VertexBuffer(data, size) {
			_layout = layout;
		}

		~VertexBuffer() {
			glDeleteBuffers(1, &_vboID);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not destroy Vertex Buffer.");
#endif
		}

		void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		}

		void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void fill(const void* data, GLsizeiptr size) {
			if (size > _size) return;

			glBindBuffer(GL_ARRAY_BUFFER, _vboID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could upload data to Vertex Buffer.");
#endif
		}

		GLsizei size() const {
			return _size;
		}

		void setLayout(const VertexBufferLayout& layout) {
			_layout = layout;
		}

		const VertexBufferLayout& layout() const {
			return _layout;
		}
	};

}