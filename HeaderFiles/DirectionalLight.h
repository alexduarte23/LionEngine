#pragma once

#include "Light.h"
#include "Shadow.h"

namespace avt {

	class DirectionalLight : public Light {
	public:
		Shadow shadow;

		DirectionalLight() : Light() {
			shadow = avt::Shadow();
		}

		DirectionalLight(avt::Vector3 pos, avt::Vector3 color) : Light(pos, color) {
			shadow = avt::Shadow((unsigned int)2048, (unsigned int)2048, avt::OrthographicCamera(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f));
			shadow.setPosition(pos);
		}

		~DirectionalLight() {}

		void setupShadows() {
			shadow.setup();
		}

		void setPosition(avt::Vector3 pos) {
			_pos = pos;
			shadow.setPosition(_pos);
			shadow.lookAt(avt::Vector3(0.0f, 0.0f, 0.0f));
		}

		void updateLightSpaceMatrices(Shader& shader, const std::string& lsmuniform) {
			shader.bind();
			shader.uploadUniformMat4(lsmuniform, shadow._lightView.projMatrix() * shadow._lightView.viewMatrix());
			shader.unbind();
		}

		void renderShadowMaps(Renderer& renderer, Scene& scene, unsigned int screenWidth, unsigned int screenHeight) {
			shadow.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
		}

		void shadowMapTextureLoad(Shader& shader, int textureUnit) {
			shader.bind();
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, shadow.depthMap());

			glActiveTexture(GL_TEXTURE0);
			shader.unbind();
		}
	};
}
