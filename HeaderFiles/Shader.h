#pragma once

#include <GL/glew.h>

#include <memory>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <initializer_list>
#include "avt_math.h"
#include "VertexBufferLayout.h"

namespace avt {

	struct ShaderInputAttr {
		std::string name;
		GLint location;
		ShaderDataType type;
		GLint length;

		bool operator<(const ShaderInputAttr& attr) const {
			return location < attr.location;
		}

		static ShaderDataType getShaderType(GLenum GLtype) {
			switch (GLtype) {
			case GL_FLOAT:		return ShaderDataType::FLOAT;
			case GL_INT:		return ShaderDataType::INT;
			case GL_BOOL:		return ShaderDataType::BOOL;
			case GL_FLOAT_VEC2:	return ShaderDataType::VEC2;
			case GL_FLOAT_VEC3:	return ShaderDataType::VEC3;
			case GL_FLOAT_VEC4:	return ShaderDataType::VEC4;
			case GL_FLOAT_MAT2:	return ShaderDataType::MAT2;
			case GL_FLOAT_MAT3:	return ShaderDataType::MAT3;
			case GL_FLOAT_MAT4:	return ShaderDataType::MAT4;
			default:			return ShaderDataType::FLOAT;
			}
		}

		static int getTypeSlots(ShaderDataType type) {
			switch (type) {
			case ShaderDataType::FLOAT:
			case ShaderDataType::INT:
			case ShaderDataType::BOOL:
			case ShaderDataType::VEC2:
			case ShaderDataType::VEC3:
			case ShaderDataType::VEC4:	return 1;
			case ShaderDataType::MAT2:	return 2;
			case ShaderDataType::MAT3:	return 3;
			case ShaderDataType::MAT4:	return 4;
			default:					return 1;
			}
		}
	};

	class ShaderInputLayout {
	private:
		std::set<ShaderInputAttr> _attrs;
	public:
		ShaderInputLayout() {}

		ShaderInputLayout(const std::initializer_list<ShaderInputAttr> inputs) {
			for (auto& attr : inputs) _attrs.insert(attr);
		}

		~ShaderInputLayout() {}

		void addAttr(const ShaderInputAttr& attr) {
			_attrs.insert(attr);
		}

		bool wellFormed() const {
			GLint loc = 0;
			for (auto& attr : _attrs) {
				if (attr.location != loc) return false;
				loc += attr.length * ShaderInputAttr::getTypeSlots(attr.type);
			}
			return true;
		}

		std::vector<ShaderInputAttr> getAttrs() const {
			std::vector<ShaderInputAttr> attrs;
			for (auto& attr : _attrs) attrs.push_back(attr);
			return attrs;
		}

	};

	class ShaderParams{
	private:
		friend class Shader;

		std::map<std::string, GLuint> _inputs;
		std::map<std::string, std::string> _macros;
		std::set<std::string> _uniforms;
		std::map<std::string, GLuint> _uniformBlocks;
		std::map<std::string, GLuint> _textures;
		std::string _vertexShader;
		std::string _fragmentShader;
		bool _externalSource = true;

		std::string _model = "";

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
			_uniforms.insert(uniform);
			return *this;
		}

		ShaderParams& addUniforms(std::initializer_list<std::string> uniforms) {
			for (auto& uniform : uniforms)
				_uniforms.insert(uniforms);
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

		ShaderParams& useModelMatrix(const std::string& uName = "ModelMatrix") {
			_uniforms.insert(uName);
			_model = uName;
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

		mutable std::unique_ptr<ShaderInputLayout> _layout;

		mutable std::map<std::string, GLint> _uniforms;
		std::string _modelUniform = "";

		GLchar* parseShader(const std::string& filename);
		unsigned int compileShader(GLenum shader_type, const std::string& source, bool external);

		void computeLayout() const;

		GLuint getUniformLocation(const std::string& uniform) const {
			auto it = _uniforms.find(uniform);
			if (it == _uniforms.end()) {
				auto location = glGetUniformLocation(_program, uniform.c_str());
				_uniforms.insert({ uniform, location });
				return location;
			} else {
				return it->second;
			}
		}

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

		const ShaderInputLayout& getInputLayout() const {
			if (!_layout) computeLayout();
			return *_layout;
		}

		void uploadModelMatrix(const Mat4& model) {
			if (_modelUniform.length() == 0) return;
			uploadUniformMat4(_modelUniform, model);
		}

		void uploadUniformFloat(const std::string& uniform, float value, bool bind = false) {
			if (bind) this->bind();
			glUniform1f(getUniformLocation(uniform), value);
		}

		void uploadUniformInt(const std::string& uniform, int value, bool bind = false) {
			if (bind) this->bind();
			glUniform1i(getUniformLocation(uniform), value);
		}

		void uploadUniformBool(const std::string& uniform, bool value, bool bind = false) {
			uploadUniformInt(uniform, value, bind);
		}

		void uploadUniformVec2(const std::string& uniform, const Vector2& vec, bool bind = false) {
			if (bind) this->bind();
			glUniform2f(getUniformLocation(uniform), vec.x, vec.y);
		}

		void uploadUniformVec3(const std::string& uniform, const Vector3& vec, bool bind = false) {
			if (bind) this->bind();
			glUniform3f(getUniformLocation(uniform), vec.x, vec.y, vec.z);
		}

		void uploadUniformVec4(const std::string& uniform, const Vector4& vec, bool bind = false) {
			if (bind) this->bind();
			glUniform4f(getUniformLocation(uniform), vec.x, vec.y, vec.z, vec.w);
		}

		void uploadUniformMat2(const std::string& uniform, const Mat2& mat, bool bind = false) {
			if (bind) this->bind();
			glUniformMatrix2fv(getUniformLocation(uniform), 1, GL_FALSE, mat.data());
		}

		void uploadUniformMat3(const std::string& uniform, const Mat3& mat, bool bind = false) {
			if (bind) this->bind();
			glUniformMatrix3fv(getUniformLocation(uniform), 1, GL_FALSE, mat.data());
		}

		void uploadUniformMat4(const std::string& uniform, const Mat4& mat, bool bind = false) {
			if (bind) this->bind();
			glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, mat.data());
		}

	};
}
