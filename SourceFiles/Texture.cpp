#include "../HeaderFiles/Texture.h"
#include <cassert>
#include "../HeaderFiles/Globals.h"

void avt::RenderTargetTexture::destroy()
{

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &_texID);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &_rboDepthStencil);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &_framebuffer);
}

void avt::RenderTargetTexture::createColorTexture(const int width, const int height)
{

	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texID, 0);
}

void avt::RenderTargetTexture::createRenderbufferObject(const int width, const int height)
{
	glGenRenderbuffers(1, &_rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboDepthStencil);
}

avt::RenderTargetTexture::RenderTargetTexture() :

	_framebuffer(-1),
	_rboDepthStencil(-1),
	_r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f)
{
}

avt::RenderTargetTexture::~RenderTargetTexture()
{
	destroy();
}

/*void avt::RenderTargetTexture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, _texID);
}

void avt::RenderTargetTexture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}*/

void avt::RenderTargetTexture::create(const int width, const int height)
{
	glGenFramebuffers(1, &_framebuffer);
	//std::cout << _framebuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	createColorTexture(width, height);
	createRenderbufferObject(width, height);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_quad.create();
}

void avt::RenderTargetTexture::setFramebufferClearColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
	_r = r; _g = g; _b = b; _a = a;
}

void avt::RenderTargetTexture::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}

void avt::RenderTargetTexture::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void avt::RenderTargetTexture::renderQuad(Shader* shader, std::string textureUniform) {

	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texID);
	shader->uploadUniformInt(textureUniform, 0);


	glDisable(GL_DEPTH_TEST);
	//creates multiple times should create just once
	_quad.draw();
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
	shader->unbind();
}

void avt::RenderTargetTexture::renderQuad(RenderTargetTexture rtt2) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texID);

	//glActiveTexture(GL_TEXTURE1);
	//rtt2.bind();

	glDisable(GL_DEPTH_TEST);
	_quad.draw();
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
}


GLfloat data[] = { 1.0f, -1.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						-1.0f, -1.0f, 0.0f, 0.0f,
							-1.0f, 1.0f, 0.0f, 1.0f };

//Quad2D
avt::Quad2D::Quad2D() : _vaoId(-1), _vboId(-1)
{
	
	memcpy(_vertices, data, sizeof(data));
}

avt::Quad2D::~Quad2D()
{
	destroy();
}

void avt::Quad2D::destroy() {
	glDeleteBuffers(1, &_vboId);
	glDeleteVertexArrays(1, &_vaoId);

}

void avt::Quad2D::create() {
	
	/**
	_va.create();
	_vb.create(_vertices.GLdata(), 16 * sizeof(GLfloat));

	VertexBufferLayout layout;
	layout.add<GLfloat>(2); // VERTICES
	layout.add<GLfloat>(2); //TEXCOORDS
	_va.addBuffer(_vb, layout, 0);
	_va.unbind();
	/**/

	glGenVertexArrays(1, &_vaoId);
	glGenBuffers(1,  &_vboId);

	glBindVertexArray(_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(TEXTURES);
	glVertexAttribPointer(TEXTURES, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

}

void avt::Quad2D::draw() {
	glBindVertexArray(_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


// TEXTURE

std::unique_ptr<avt::Texture> avt::Texture::_default(nullptr);

void avt::Texture::create(const std::string& filename, const TextureParams& params) {
	if (_texID) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &_texID);
	}

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.data(), &_width, &_height, &_nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture" << std::endl;
		return;
	}

	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params._wrap[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params._wrap[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params._filter[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params._filter[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, _nrChannels == 4 ? GL_RGBA8 : GL_RGB8, _width, _height, 0, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	if (params._useMipmap) glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	_params = params;

#ifndef ERROR_CALLBACK
	ErrorManager::checkOpenGLError("ERROR: Could not create texture.");
#endif
}

void avt::Texture::create(int width, int height, const TextureParams& params) {
	if (_texID) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &_texID);
	}

	_width = width;
	_height = height;
	_nrChannels = 4;

	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params._wrap[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params._wrap[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params._filter[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params._filter[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//if (params._useMipmap) glGenerateMipmap(GL_TEXTURE_2D);

	_params = params;

#ifndef ERROR_CALLBACK
	ErrorManager::checkOpenGLError("ERROR: Could not create empty texture.");
#endif
}

void avt::Texture::clear() {
	if (!_texID) return;

	std::vector<unsigned char> fill((size_t)_width * _height * _nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, fill.data());
}

void avt::Texture::mixImage(const std::string& filename, const Vector2& start) {
	int w, h, channels;

	if (!_texID) return;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.data(), &w, &h, &channels, _nrChannels);

	if (!data) {
		std::cerr << "Failed to load texture image" << std::endl;
		return;
	}
	else if (start.x + w > _width || start.y + h > _height) {
		std::cerr << "Texture image doesn't fit" << std::endl;
		stbi_image_free(data);
		return;
	}


	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)start.x, (GLint)start.y, w, h, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	if (_params._useMipmap) glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

#ifndef ERROR_CALLBACK
	ErrorManager::checkOpenGLError("ERROR: Could not update texture data.");
#endif
}

void avt::Texture::replaceImage(const std::string& filename) {
	if (!_texID) return;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.data(), &_width, &_height, &_nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture image" << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexImage2D(GL_TEXTURE_2D, 0, _nrChannels == 4 ? GL_RGBA8 : GL_RGB8, _width, _height, 0, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	if (_params._useMipmap) glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

#ifndef ERROR_CALLBACK
	ErrorManager::checkOpenGLError("ERROR: Could not replace texture data.");
#endif
}




void avt::MultipleRenderTarget::destroy()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(2, colorBuffers);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &_rboDepthStencil);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &_framebuffer);
}

void avt::MultipleRenderTarget::createColorTexture(const int width, const int height)
{

	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	GLenum  attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
}

void avt::MultipleRenderTarget::createRenderbufferObject(const int width, const int height)
{
	glGenRenderbuffers(1, &_rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboDepthStencil);
}

avt::MultipleRenderTarget::MultipleRenderTarget() : 
	_framebuffer(-1),
	_rboDepthStencil(-1),
	_r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f)
{
}

avt::MultipleRenderTarget::~MultipleRenderTarget()
{
	destroy();
}

void avt::MultipleRenderTarget::create(const int width, const int height)
{
	glGenFramebuffers(1, &_framebuffer);
	//std::cout << _framebuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	createColorTexture(width, height);
	createRenderbufferObject(width, height);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_quad.create();
}


void avt::MultipleRenderTarget::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void avt::MultipleRenderTarget::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void avt::MultipleRenderTarget::renderQuad(Shader* shader, std::string textureUniform)
{
	shader->bind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
	shader->uploadUniformInt(textureUniform, 1);


	glDisable(GL_DEPTH_TEST);
	//creates multiple times should create just once
	_quad.draw();
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
	shader->unbind();
}

void avt::MultipleRenderTarget::renderAll(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDisable(GL_DEPTH_TEST);
	//creates multiple times should create just once
	_quad.draw();
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
}
