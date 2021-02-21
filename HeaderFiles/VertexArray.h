#pragma once


#include <GL/glew.h>
#include <vector>
#include <memory>
#include "../HeaderFiles/VertexBuffer.h"
#include "../HeaderFiles/IndexBuffer.h"
#include "../HeaderFiles/VertexBufferLayout.h"

namespace avt {

	class VertexArray {
	private:
		GLuint _vaoID;
		unsigned int _attribNum;
		std::vector<std::shared_ptr<VertexBuffer>> _vbs;
		std::shared_ptr<IndexBuffer> _ib;

	public:
		VertexArray() : _vaoID(0), _attribNum(0) {
			glGenVertexArrays(1, &_vaoID);
			glBindVertexArray(_vaoID);
			glBindVertexArray(0);

#ifndef ERROR_CALLBACK
			ErrorManager::checkOpenGLError("ERROR: Could not create Vertex Array.");
#endif
		}

		~VertexArray();

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb, bool instanced = false);
		
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ib);

		bool indexed() const {
			return _ib.get() != nullptr;
		}

		const std::shared_ptr<IndexBuffer>& ib() const {
			return _ib;
		}

		const std::vector<std::shared_ptr<VertexBuffer>>& vbs() const {
			return _vbs;
		}

		void bind() const {
			glBindVertexArray(_vaoID);
		}

		void unbind() const {
			glBindVertexArray(0);
		}
	};

}