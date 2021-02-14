#include "../HeaderFiles/HUDElement.h"

namespace avt {

	const float HUDElement::QUAD_STRIP[4 * 9] = { // pos tex color
			-1.f, -1.f, 0,      0,   0,   1.f, 1.f, 1.f, 1.f, // bottom left
			 1.f, -1.f, 0,    1.f,   0,   1.f, 1.f, 1.f, 1.f, // bottomm right
			-1.f,  1.f, 0,      0, 1.f,   1.f, 1.f, 1.f, 1.f, // top left
			 1.f,  1.f, 0,    1.f, 1.f,   1.f, 1.f, 1.f, 1.f // top right
	};


	HUDElement::HUDElement(const std::string& textureFilename) {
		_sprite.create(textureFilename);

		_va.create();

		VertexBufferLayout layout({
			{ShaderDataType::VEC3, "position"},
			{ShaderDataType::VEC2, "texCoord"},
			{ShaderDataType::VEC4, "color"}
		});

		_vb.create(QUAD_STRIP, sizeof(QUAD_STRIP));
		_va.addVertexBuffer(_vb, layout);

		_vb.unbind();
		_va.unbind();

	}

	void HUDElement::draw(Shader* shader, const Mat4& worldMatrix) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = getShader() ? getShader() : shader;
		if (getShader()) {
			getShader()->bind();
		}

		_va.bind();
		_sprite.bind();

		beforeDraw();
		glDisable(GL_DEPTH_TEST);
		curr_shader->uploadUniformMat4(MODEL_MATRIX, newWorldMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glEnable(GL_DEPTH_TEST);
		afterDraw();

		_sprite.unbind();

		_va.unbind();

		if (getShader()) {
			getShader()->unbind();
			shader->bind();
		}

	}

}