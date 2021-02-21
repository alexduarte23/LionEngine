#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/ErrorManager.h"

namespace avt {

	GLchar* Shader::parseShader(const std::string& filename) {
		std::ifstream fileStream(filename);
		std::string shader;
		std::string line;

		while (std::getline(fileStream, line)) {
			shader.append(line + '\n');
		}
		if (fileStream.eof()) {
			auto output = new GLchar[shader.length() + 1];
			strcpy_s(output, shader.length() + 1, shader.c_str());
			return output;
		}
		else {
			std::cout << "Error reading file " << filename << std::endl;
			return nullptr;
		}

	}

	unsigned int Shader::compileShader(GLenum shader_type, const std::string& source, bool external) {
		const GLchar* shaderChar;
		if (external) shaderChar = parseShader(source);
		else shaderChar = source.c_str();

		unsigned int shaderId = glCreateShader(shader_type);
		glShaderSource(shaderId, 1, &shaderChar, 0);
		glCompileShader(shaderId);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not compile shader.");
#endif

		delete[] shaderChar;
		return shaderId;
	}


	Shader::Shader(const ShaderParams& params) {
		std::vector<GLuint> shaderIDs;
		shaderIDs.push_back(compileShader(GL_VERTEX_SHADER, params._vertexShader, params._externalSource));
		shaderIDs.push_back(compileShader(GL_FRAGMENT_SHADER, params._fragmentShader, params._externalSource));

		_program = glCreateProgram();

		for (auto& el : shaderIDs) { // Shader Attach
			glAttachShader(_program, el);
		}

		for (auto& el : params._inputs) { // Attribute Bind
			glBindAttribLocation(_program, el.second, el.first.c_str());
		}

		glLinkProgram(_program);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not link shader program.");
#endif

		for (auto& el : params._uniforms) { // Uniforms
			auto index = glGetUniformLocation(_program, el.c_str());
			_uniforms.insert({ el, index });
		}

		for (auto& el : params._uniformBlocks) { // UBOS
			auto block_index = glGetUniformBlockIndex(_program, el.first.c_str());
			glUniformBlockBinding(_program, block_index, el.second);
		}

		for (auto& el : shaderIDs) { // Shader Delete
			glDetachShader(_program, el);
			glDeleteShader(el);
		}

		glUseProgram(_program);
		for (auto& tex : params._textures) { // set textures
			auto location = glGetUniformLocation(_program, tex.first.c_str());
			glUniform1i(location, tex.second);
		}
		glUseProgram(0);
	}

}