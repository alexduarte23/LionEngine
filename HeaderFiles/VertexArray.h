#pragma once


#include <GL/glew.h>
#include "../HeaderFiles/VertexBuffer.h"
#include "../HeaderFiles/IndexBuffer.h"
#include "../HeaderFiles/VertexBufferLayout.h"

namespace avt {

	class VertexArray {
	private:
		GLuint _vaoID;
		unsigned int _attribNum;
		bool _indexed;
	public:
		VertexArray() : _vaoID(0), _attribNum(0), _indexed(false) {}
		~VertexArray();

		void create();

		void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool instanced = false);
		
		void setIndexBuffer(const IndexBuffer& ib);

		bool indexed() const {
			return _indexed;
		}

		void bind() const {
			glBindVertexArray(_vaoID);
		}


		void unbind() const {
			glBindVertexArray(0);
		}
	};

}