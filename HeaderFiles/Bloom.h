#pragma once
#include <GL\glew.h>
#include "avt_math.h"
#include "Shader.h"
#include "Texture.h"
#include "Globals.h"

namespace avt {

	class Bloom {
	private:
		MultipleRenderTarget _HDR;
		RenderTargetTexture _pingBlur;
		RenderTargetTexture _pongBlur;

		Shader _ShaderBrightValues;
		Shader _ShaderGaussianBlur;
		Shader _ShaderBloomFinal;

		unsigned int _amount = 2;
		bool _bloom = true;
		GLfloat exposure = 1.f;

		void createShaders() {
			createShaderBrightValues();
			createShaderGaussianBlur();
			createShaderBloomFinal();
		}

		void createShaderBrightValues() {
			ShaderParams params;
			params.setVertexShader("./Resources/bloomShaders/brightVertexshader.shader")
				.setFragmentShader("./Resources/bloomShaders/brightFragmentshader.shader")
				.addInputs({ "inVertex", "inTexcoord" })
				.addUniform("TexFramebuffer");
			_ShaderBrightValues.create(params);
		}

		void createShaderGaussianBlur() {
			ShaderParams params;
			params.setVertexShader("./Resources/bloomShaders/gaussianblurVertexshader.shader")
				.setFragmentShader("./Resources/bloomShaders/gaussianblurFragmentshader.shader")
				.addInputs({ "inVertex", "inTexcoord" })
				.addUniforms({ "TexFramebuffer", "horizontal", "tex" });
			_ShaderGaussianBlur.create(params);
		}

		void createShaderBloomFinal() {
			ShaderParams params;
			params.setVertexShader("./Resources/bloomShaders/bloomFinalVertexshader.shader")
				.setFragmentShader("./Resources/bloomShaders/bloomFinalFragmentshader.shader")
				.addInputs({ "inVertex", "inTexcoord" })
				.addTextures({ "scene", "bloomBlur" })
				.addUniforms({ "bloom", "exposure" });
			_ShaderBloomFinal.create(params);
		}

	public:
		void create(int width, int height) {

			//Framebuffers
			_HDR.create(width, height);
			_pingBlur.create(width, height);
			_pongBlur.create(width, height);
			//Shaders
			createShaders();
		}

		void renderHDR() {
			_HDR.renderQuad(&_ShaderBrightValues, "TexFramebuffer");
		}

		void turnOffOnBloom() {
			_bloom = !_bloom;
		}

		void renderBlur() {
			bool horizontal = true;
			

			for (unsigned int i = 0; i < _amount; i++)
			{
				_pongBlur.bindFramebuffer();
				_ShaderGaussianBlur.uploadUniformInt("horizontal", horizontal, true);
				_pingBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
				_pongBlur.unbindFramebuffer();
				horizontal = !horizontal;

				_pingBlur.bindFramebuffer();
				_ShaderGaussianBlur.uploadUniformInt("horizontal", horizontal, true);
				_pongBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
				_pingBlur.unbindFramebuffer();
				horizontal = !horizontal;

			}
			
			_pingBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
		}

		void setBlurTex(int t) {
			if (t < 0)
				if(_amount > 0)
					_amount += t;
			if (t >= 0)
				if (_amount < 20)
					_amount += t;
		}

		void renderBloomFinal() {

			_ShaderBloomFinal.bind();
			_ShaderBloomFinal.uploadUniformBool("bloom", _bloom);
			_ShaderBloomFinal.uploadUniformFloat("exposure", exposure);

			_HDR.renderAll(_pingBlur.getId());
			//_pingBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
			//_HDR.renderQuad(&_ShaderBloomFinal, "scene");
			_ShaderBloomFinal.unbind();

		}


		void bindHDR() {
			_HDR.bindFramebuffer();
		}

		void unbindHDR() {
			_HDR.unbindFramebuffer();
		}

		void bindPingBlur() {
			_pingBlur.bindFramebuffer();
		}

		void unbindPingBlur() {
			_pingBlur.unbindFramebuffer();
		}

		void bindPongBlur() {
			_pongBlur.bindFramebuffer();
		}

		void unbindPongBlur() {
			_pongBlur.unbindFramebuffer();
		}

	};
}
