#include "..\HeaderFiles\VertexArray.h"

#include "..\HeaderFiles\ErrorManager.h"

namespace avt {

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

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb, bool instanced) {
		glBindVertexArray(_vaoID);
		vb->bind();

		auto& layout = vb->layout();
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
		_vbs.push_back(vb);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not add Vertex Buffer to Vertex Array.");
#endif
	}

	void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) {
		glBindVertexArray(_vaoID);
		ib->bind();
		glBindVertexArray(0);
		_ib = ib;

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not add Index Buffer to Vertex Array.");
#endif
	}

}