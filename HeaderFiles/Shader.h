#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <initializer_list>


namespace avt {

	const std::string MODEL_MATRIX = "ModelMatrix";
	const std::string NORMAL_MATRIX = "NormalMatrix";
	const std::string VIEW_MATRIX = "ViewMatrix";
	const std::string PROJECTION_MATRIX = "ProjectionMatrix";

	const std::string VERTEX_ATTRIBUTE = "inVertex";
	const std::string NORMAL_ATTRIBUTE = "inNormal";
	const std::string TEXCOORD_ATTRIBUTE = "inTexcoord";
	const std::string TANGENT_ATTRIBUTE = "inTangent";
	const std::string BITANGENT_ATTRIBUTE = "inBitangent";

	class ShaderParams{
	private:
		friend class Shader;

		std::map<std::string, GLuint> _inputs;
		std::map<std::string, std::string> _macros;
		std::vector<std::string> _uniforms;
		std::map<std::string, GLuint> _uniformBlocks;
		std::map<std::string, GLuint> _textures;
		std::string _vertexShader;
		std::string _fragmentShader;

	public:

		ShaderParams& addInput(std::string attrib, GLuint location) {
			_inputs.insert({ attrib, location });
			return *this;
		}

		ShaderParams& addInputs(std::initializer_list<std::string> attribs, GLuint startLocation = 0) {
			auto loc = startLocation;
			for (auto& attr : attribs)
				_inputs.insert({ attr, loc++ });
			return *this;
		}

		ShaderParams& addUniform(std::string uniform) {
			_uniforms.push_back(uniform);
			return *this;
		}

		ShaderParams& addUniforms(std::initializer_list<std::string> uniforms) {
			_uniforms.insert(_uniforms.end(), uniforms.begin(), uniforms.end());
			return *this;
		}

		ShaderParams& addUniformBlock(std::string ub, GLuint bindingPoint) {
			_uniformBlocks.insert({ ub, bindingPoint });
			return *this;
		}

		ShaderParams& addUniformsBlocks(std::initializer_list<std::string> ubs, GLuint startBP = 0) {
			auto bp = startBP;
			for (auto& ub : ubs)
				_uniformBlocks.insert({ ub, bp++ });
			return *this;
		}

		ShaderParams& addTexture(std::string tex, GLuint bindingPoint) {
			_textures.insert({ tex, bindingPoint });
			return *this;
		}

		ShaderParams& addTexture(std::initializer_list<std::string> texs, GLuint startBP = 0) {
			auto bp = startBP;
			for (auto& tex : texs)
				_textures.insert({ tex, bp++ });
			return *this;
		}

		ShaderParams& setVertexShader(std::string filename) {
			_vertexShader = filename;
			return *this;
		}

		ShaderParams& setFragmentShader(std::string filename) {
			_fragmentShader = filename;
			return *this;
		}

		ShaderParams& addMacro(std::string macro, std::string value) {
			_macros.insert({macro, value});
			return *this;
		}

		ShaderParams& addMacros(std::initializer_list<std::pair<std::string, std::string>> macros) {
			for (auto& mac : macros)
				_macros.insert(mac);
			return *this;
		}

		ShaderParams& clearInputs() {
			_inputs.clear();
			return *this;
		}

		ShaderParams& clearMacros() {
			_macros.clear();
			return *this;
		}

		ShaderParams& clearUniforms() {
			_uniforms.clear();
			return *this;
		}

		ShaderParams& clearUniformBlocks() {
			_uniformBlocks.clear();
			return *this;
		}

		ShaderParams& clear() {
			_inputs.clear();
			_macros.clear();
			_uniforms.clear();
			_uniformBlocks.clear();
			_vertexShader.clear();
			_fragmentShader.clear();
			return *this;
		}

	};

	class Shader {
	private:
		GLuint _program;

		std::map<std::string, GLint> _uniforms;

		void parseShader(const std::string& filename, GLchar** output);
		unsigned int compileShader(GLenum shader_type, const std::string& filename);

	public:
		Shader() : _program(0) {}
		~Shader() {}

		void create(const ShaderParams& params);

		GLuint getUniform(const std::string& name) {
			return _uniforms.find(name)->second;
		}

		void bind() {
			glUseProgram(_program);
		}

		void unbind() {
			glUseProgram(0);
		}

	};
}
