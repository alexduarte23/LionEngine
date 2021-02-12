#include "..\HeaderFiles\VertexArray.h"

#include "..\HeaderFiles\ErrorManager.h"

namespace avt {

	void VertexArray::create() {
		glGenVertexArrays(1, &_vaoID);
		glBindVertexArray(_vaoID);
		glBindVertexArray(0);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not create Vertex Array.");
#endif
	}

	VertexArray::~VertexArray() {
		glBindVertexArray(_vaoID);
		for (unsigned int i = 0; i < _attribNum; i++) {
			glDisableVertexAttribArray(i);
		}
		glDeleteVertexArrays(1, &_vaoID);
		glBindVertexArray(0);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not destroy Vertex Array.");
#endif
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool instanced) {
		if (!_vaoID) return;

		glBindVertexArray(_vaoID);
		vb.bind();

		for (const auto& el : layout) {
			switch (el.type) {
			case avt::ShaderDataType::INT:
			case avt::ShaderDataType::BOOL:
				glEnableVertexAttribArray(_attribNum);
				glVertexAttribIPointer(_attribNum, el.count, el.GLtype, layout.stride(), (const void*)el.offset);
				if (instanced) glVertexAttribDivisor(_attribNum, 1);
				_attribNum++;
				break;
			case avt::ShaderDataType::FLOAT:
			case avt::ShaderDataType::VEC2:
			case avt::ShaderDataType::VEC3:
			case avt::ShaderDataType::VEC4:
				glEnableVertexAttribArray(_attribNum);
				glVertexAttribPointer(_attribNum, el.count, el.GLtype, el.norm, layout.stride(), (const void*)el.offset);
				if (instanced) glVertexAttribDivisor(_attribNum, 1);
				_attribNum++;
				break;
			case avt::ShaderDataType::MAT2:
			case avt::ShaderDataType::MAT3:
			case avt::ShaderDataType::MAT4:
				for (uint8_t i = 0; i < el.count; i++) {
					glEnableVertexAttribArray(_attribNum);
					glVertexAttribPointer(_attribNum, el.count, el.GLtype, el.norm, layout.stride(), (const void*)(el.offset + sizeof(GLfloat) * el.count * i));
					if (instanced) glVertexAttribDivisor(_attribNum, 1);
					_attribNum++;
				}
			}
		}
		glBindVertexArray(0);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not add Vertex Buffer to Vertex Array.");
#endif
	}

	void VertexArray::setIndexBuffer(const IndexBuffer& ib) {
		if (!_vaoID) return;

		glBindVertexArray(_vaoID);
		ib.bind();
		glBindVertexArray(0);
		_indexed = true;

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not add Index Buffer to Vertex Array.");
#endif
	}

}