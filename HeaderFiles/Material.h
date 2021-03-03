#pragma once

#include <memory>
#include "Shader.h"
#include "Texture.h"


namespace avt {

	class Material {
	private:
		std::shared_ptr<Shader> _shader;
		std::shared_ptr<Texture> _texture;

	public:
		Material(const std::shared_ptr<Shader>& shader) : _shader(shader) {}

		~Material() {}

		const std::shared_ptr<Shader>& shader() const {
			return _shader;
		}

		void bind() const {
			_shader->bind();
		}

		void unbind() const {
			_shader->unbind();
		}
	};


}