#include "../HeaderFiles/Texture.h"
#include <cassert>


namespace avt {

	Texture::Texture(const std::string& filename, const TextureParams& params) {
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

	Texture::Texture(int width, int height, const TextureParams& params) {
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

	void Texture::clear() {
		std::vector<unsigned char> fill((size_t)_width * _height * _nrChannels, 0);
		glBindTexture(GL_TEXTURE_2D, _texID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, fill.data());
	}

	void Texture::mixImage(const std::string& filename, const Vector2& start) {
		int w, h, channels;

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

	void Texture::replaceImage(const std::string& filename) {
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

}