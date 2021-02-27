#pragma once

#include <GL/glew.h>
#include <vector>
#include "ErrorManager.h"
#include "VertexBufferLayout.h"

namespace avt{

	class VertexBuffer {
	private:
		GLuint _vboID;
		GLsizei _size;
		GLsizei _capacity;
		VertexBufferLayout _layout;

	public:
		// main constructors
		VertexBuffer(const void* data, GLsizeiptr size)
			: _vboID(0), _size(data ? (GLsizei)size : 0), _capacity((GLsizei)size) {
			glGenBuffers(1, &_vboID);
			glBindBuffer(GL_ARRAY_BUFFER, _vboID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Vertex Buffer.");
#endif
		}

		VertexBuffer(const void* data, GLsizeiptr size, const VertexBufferLayout& layout)
			: VertexBuffer(data, size) {
			_layout = layout;
		}

		// vector constructors
		template<typename T>
		VertexBuffer(const std::vector<T>& vertices)
			: VertexBuffer(vertices.data(), vertices.size() * sizeof(T)) {}

		template<typename T>
		VertexBuffer(const std::vector<T>& vertices, const VertexBufferLayout& layout)
			: VertexBuffer(vertices.data(), vertices.size() * sizeof(T), layout) {}

		// empty content constructors
		VertexBuffer(GLsizeiptr capacity)
			: VertexBuffer(nullptr, capacity) {}

		VertexBuffer(GLsizeiptr capacity, const VertexBufferLayout& layout)
			: VertexBuffer(nullptr, capacity, layout) {}



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

		void upload(const void* data, GLsizeiptr size) {
			if (!data || size <= 0) return;
			if (size > _capacity) {
				std::cerr << "Vertex Buffer data upload FAIL: data size larger than buffer size." << std::endl;
				return;
			}

			glBindBuffer(GL_ARRAY_BUFFER, _vboID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
			_size = (GLsizei)size;

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not upload data to Vertex Buffer.");
#endif
		}

		template<typename T>
		void upload(const std::vector<T>& vertices) {
			upload(vertices.data(), vertices.size() * sizeof(T));
		}

		void resize(GLsizeiptr capacity, bool keepData = true) {
			if (capacity == _capacity && keepData || capacity <= 0) return;

			if (keepData) {
				if (capacity < _size) _size = (GLsizei)capacity;
				// create aux buffer
				GLuint auxID;
				glGenBuffers(1, &auxID);
				glBindBuffer(GL_COPY_WRITE_BUFFER, auxID);
				glBufferData(GL_COPY_WRITE_BUFFER, _size, nullptr, GL_STATIC_DRAW);

				// copy data to aux
				glBindBuffer(GL_ARRAY_BUFFER, _vboID);
				glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _size);

				// re-allocate vb and copy data from aux
				glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_STATIC_DRAW);
				glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, _size);

				// delete aux buffer
				glDeleteBuffers(1, &auxID);
				glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
			} else {
				glBindBuffer(GL_ARRAY_BUFFER, _vboID);
				glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_STATIC_DRAW);
				_size = 0;
			}
			_capacity = (GLsizei)capacity;

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not resize Vertex Buffer.");
#endif
		}

		void clear() {
			_size = 0;
		}

		GLsizei size() const {
			return _size;
		}

		GLsizei count() const {
			return _layout.stride() == 0 ? _size : _size / _layout.stride();
		}

		GLsizei capacity() const {
			return _capacity;
		}

		GLsizei capacityCount() const {
			return _layout.stride() == 0 ? _capacity : _capacity / _layout.stride();
		}

		void setLayout(const VertexBufferLayout& layout) {
			_layout = layout;
		}

		const VertexBufferLayout& layout() const {
			return _layout;
		}
	};

}