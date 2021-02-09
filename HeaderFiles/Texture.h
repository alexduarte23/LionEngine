#pragma once
#include <GL\glew.h>
#include "VertexArray.h"
#include "avt_math.h"
#include "Shader.h"
#include "ErrorManager.h"

#include "../Dependencies/stb_image.h"
#include <string>
#include <vector>

namespace avt {

	class Texture;
	class RenderTargetTexture;
	class MultipleRenderTarget;
	class Quad2D;



	/*class Texture {
	protected:
		GLuint id;
	public:
		Texture() : id(-1) {}
		virtual void bind();
		virtual void unbind();
	};*/

	class TextureParams {
	private:
		friend class Texture;

		GLenum _wrap[2] = { GL_REPEAT, GL_REPEAT };
		GLenum _filter[2] = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
		bool _useMipmap = true;

	public:
		TextureParams& wrap(GLenum wrapS, GLenum wrapT = 0) {
			_wrap[0] = wrapS;
			_wrap[1] = wrapT ? wrapT : wrapS;
			return *this;
		}

		TextureParams& wrapS(GLenum wrapS) {
			_wrap[0] = wrapS;
			return *this;
		}

		TextureParams& wrapT( GLenum wrapT) {
			_wrap[1] = wrapT;
			return *this;
		}

		TextureParams& filter(GLenum filterMin, GLenum filterMag = 0) {
			_filter[0] = filterMin;
			_filter[1] = filterMag ? filterMag : filterMin;
			_useMipmap = filterMin == GL_NEAREST_MIPMAP_LINEAR || filterMin == GL_NEAREST_MIPMAP_NEAREST
				|| filterMin == GL_LINEAR_MIPMAP_LINEAR || filterMin == GL_LINEAR_MIPMAP_NEAREST;
			return *this;
		}

		TextureParams& filterMin(GLenum filterMin) {
			_filter[0] = filterMin;
			return *this;
		}

		TextureParams& filterMag(GLenum filterMag) {
			_filter[1] = filterMag;
			return *this;
		}

		TextureParams& reset() {
			_wrap[0] = GL_REPEAT;
			_wrap[1] = GL_REPEAT;
			_filter[0] = GL_LINEAR_MIPMAP_LINEAR;
			_filter[1] = GL_LINEAR;
			return *this;
		}
	};

	class Texture {
	private:
		static std::unique_ptr<Texture> _default;

	protected:
		GLuint _texID;

		int _width, _height, _nrChannels;
		TextureParams _params;

	public:
		Texture()
			: _texID(0), _width(0), _height(0), _nrChannels(0) {}

		~Texture() {
			if (_texID) {
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteTextures(1, &_texID);
			}
		}

		void create(const std::string& filename, const TextureParams& params = TextureParams());

		void create(int width, int height, const TextureParams& params = TextureParams());

		void clear();

		void mixImage(const std::string& filename, const Vector2& start = Vector2());

		void replaceImage(const std::string& filename);

		virtual void bind(unsigned int slot = 0) const {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, _texID);
		}

		virtual void unbind(unsigned int slot = 0) const {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		static const Texture* getDefault() {
			if (!_default.get()) {
				_default.reset(new Texture());
				_default->create("Resources/textures/default.png", TextureParams().wrap(GL_REPEAT).filter(GL_NEAREST));
			}
			return _default.get();
		}
	};


	class Quad2D : Texture {
	private:
		GLuint _vaoId;
		GLuint _vboId;
		GLfloat _vertices[16];
		void destroy();

	public:
		Quad2D();
		~Quad2D();
		void create();
		void draw();
	};

	class RenderTargetTexture : Texture {
	private:
		Quad2D _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		void destroy();
		void createColorTexture(const int width, const int height);
		void createRenderbufferObject(const int width, const int height);

	public:
		RenderTargetTexture();
		~RenderTargetTexture();
		//void bind() const override;
		//void unbind() const override;
		void create(const int width, const int height);
		void setFramebufferClearColor(const GLfloat e, const GLfloat g, const GLfloat b, const GLfloat a);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(Shader* shader, std::string textureUniform);
		void renderQuad(RenderTargetTexture rtt);
		GLuint getId() {
			return _texID;
		}
	};

	class MultipleRenderTarget : Texture {
	private:
		Quad2D _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		GLuint colorBuffers[2];
		void destroy();
		void createColorTexture(const int width, const int height);
		void createRenderbufferObject(const int width, const int height);

	public:
		MultipleRenderTarget();
		~MultipleRenderTarget();
		void create(const int width, const int height);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(Shader* shader, std::string textureUniform);
		void renderAll(GLuint texture);
		GLuint getId() {
			return _texID;
		}
	};

}

