#pragma once

#include <GL/glew.h>

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <initializer_list>
#include "avt_math.h"


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
		bool _externalSource = true;

	public:

		ShaderParams& externalSource(bool external = true) {
			_externalSource = external;
			return *this;
		}

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

		ShaderParams& addTextures(std::initializer_list<std::string> texs, GLuint startBP = 0) {
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

		GLchar* parseShader(const std::string& filename);
		unsigned int compileShader(GLenum shader_type, const std::string& source, bool external);

	public:
		Shader(const ShaderParams& params);

		~Shader() {
			glDeleteProgram(_program);
			glUseProgram(0);
		}

		void bind() {
			glUseProgram(_program);
		}

		void unbind() {
			glUseProgram(0);
		}

		void uploadUniformFloat(const std::string& uniform, float value, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniform1f(location, value);
		}

		void uploadUniformInt(const std::string& uniform, int value, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniform1i(location, value);
		}

		void uploadUniformBool(const std::string& uniform, bool value, bool bind = false) {
			uploadUniformInt(uniform, value, bind);
		}

		void uploadUniformVec2(const std::string& uniform, const Vector2& vec, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniform2f(location, vec.x, vec.y);
		}

		void uploadUniformVec3(const std::string& uniform, const Vector3& vec, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniform3f(location, vec.x, vec.y, vec.z);
		}

		void uploadUniformVec4(const std::string& uniform, const Vector4& vec, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
		}

		void uploadUniformMat2(const std::string& uniform, const Mat2& mat, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniformMatrix2fv(location, 1, GL_FALSE, mat.data());
		}

		void uploadUniformMat3(const std::string& uniform, const Mat3& mat, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniformMatrix3fv(location, 1, GL_FALSE, mat.data());
		}

		void uploadUniformMat4(const std::string& uniform, const Mat4& mat, bool bind = false) {
			GLuint location = _uniforms.find(uniform)->second;
			if (bind) this->bind();
			glUniformMatrix4fv(location, 1, GL_FALSE, mat.data());
		}

	};
}
