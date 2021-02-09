#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

namespace avt {

	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct LayoutEl {
		GLint count;
		GLenum type;
		GLuint typeSize;
		GLboolean norm;
	};

	class VertexBufferLayout {
	private:
		std::vector<LayoutEl> _elements;
		GLsizei _stride;
		GLuint _divisor;
	public:
		VertexBufferLayout() : _stride(0), _divisor(0) {}
		~VertexBufferLayout() {}

		template<typename T>
		void add(GLint count = 1, GLboolean norm = GL_FALSE) {
			std::cerr << "layout type not supported" << std::endl;
		}

		template<>
		void add<GLfloat>(GLint count, GLboolean norm) {
			_elements.push_back({ count, GL_FLOAT, sizeof(GLfloat), norm });
			_stride += count * sizeof(GLfloat);
		}

		template<>
		void add<Vector2>(GLint count, GLboolean norm) {
			add<GLfloat>(2 * count, norm);
		}

		template<>
		void add<Vector3>(GLint count, GLboolean norm) {
			add<GLfloat>(3 * count, norm);
		}

		template<>
		void add<Vector4>(GLint count, GLboolean norm) {
			add<GLfloat>(4 * count, norm);
		}

		void setDivisor(GLuint d) {
			_divisor = d;
		}

		GLsizei stride() const {
			return _stride;
		}

		GLuint divisor() const {
			return _divisor;
		}

		const std::vector<LayoutEl>& elements() const {
			return _elements;
		}
	};

}