#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

namespace avt {

	struct Vector2;
	struct Vector3;
	struct Vector4;

	enum class ShaderDataType {
		FLOAT, INT, BOOL, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4
	};

	struct LayoutElement {
		std::string name;
		ShaderDataType type;
		long long int offset;
		GLint count;
		GLenum GLtype;
		GLuint typeSize;
		GLboolean norm;

		LayoutElement(ShaderDataType type, const std::string& name, bool norm = false)
			: name(name), type(type), offset(0), count(getTypeCount(type)), GLtype(getTypeEnum(type)), typeSize(getTypeSize(type)), norm(norm) {}

		static GLint getTypeSize(ShaderDataType type) {
			switch (type) {
			case avt::ShaderDataType::FLOAT:	return 4;
			case avt::ShaderDataType::INT:		return 4;
			case avt::ShaderDataType::BOOL:		return 1;
			case avt::ShaderDataType::VEC2:		return 4 * 2;
			case avt::ShaderDataType::VEC3:		return 4 * 3;
			case avt::ShaderDataType::VEC4:		return 4 * 4;
			case avt::ShaderDataType::MAT2:		return 4 * 2 * 2;
			case avt::ShaderDataType::MAT3:		return 4 * 3 * 3;
			case avt::ShaderDataType::MAT4:		return 4 * 4 * 4;
			default:							return 4;
			}
		}

		static GLenum getTypeEnum(ShaderDataType type) {
			switch (type) {
			case avt::ShaderDataType::FLOAT:	return GL_FLOAT;
			case avt::ShaderDataType::INT:		return GL_INT;
			case avt::ShaderDataType::BOOL:		return GL_UNSIGNED_BYTE;
			case avt::ShaderDataType::VEC2:		return GL_FLOAT;
			case avt::ShaderDataType::VEC3:		return GL_FLOAT;
			case avt::ShaderDataType::VEC4:		return GL_FLOAT;
			case avt::ShaderDataType::MAT2:		return GL_FLOAT;
			case avt::ShaderDataType::MAT3:		return GL_FLOAT;
			case avt::ShaderDataType::MAT4:		return GL_FLOAT;
			default:							return GL_FLOAT;
			}
		}

		static GLint getTypeCount(ShaderDataType type) {
			switch (type) {
			case avt::ShaderDataType::FLOAT:	return 1;
			case avt::ShaderDataType::INT:		return 1;
			case avt::ShaderDataType::BOOL:		return 1;
			case avt::ShaderDataType::VEC2:		return 2;
			case avt::ShaderDataType::VEC3:		return 3;
			case avt::ShaderDataType::VEC4:		return 4;
			case avt::ShaderDataType::MAT2:		return 2; // 2 vecs
			case avt::ShaderDataType::MAT3:		return 3; // 3 vecs
			case avt::ShaderDataType::MAT4:		return 4; // 4 vecs
			default:							return 1;
			}
		}
	};

	class VertexBufferLayout {
	private:
		std::vector<LayoutElement> _elements;
		GLsizei _stride;
	public:
		VertexBufferLayout() : _stride(0) {}
		~VertexBufferLayout() {}

		VertexBufferLayout(std::initializer_list<LayoutElement> elements) : _elements(elements), _stride(0) {
			GLuint offset = 0;
			for (auto& el : _elements) {
				el.offset = offset;
				offset += el.typeSize;
			}
			_stride = offset;
		}

		void add(const LayoutElement& el) {
			_elements.push_back(el);
			_elements.back().offset = _stride;
			_stride += el.typeSize;
		}

		GLsizei stride() const {
			return _stride;
		}

		std::vector<LayoutElement>::iterator begin() { return _elements.begin(); }
		std::vector<LayoutElement>::iterator end() { return _elements.end(); }
		std::vector<LayoutElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<LayoutElement>::const_iterator end() const { return _elements.end(); }
	};

}