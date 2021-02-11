#pragma once
#include "avt_math.h"

namespace avt {

	class Light {
	protected:
		avt::Vector3 _pos;
		avt::Vector3 _color;
		float _intensity;

	public:

		Light(): _pos({0.0f, 0.0f, 0.0f}), _color(1.f, 1.f, 1.f), _intensity(1.f){}

		Light(avt::Vector3 pos, avt::Vector3 color): _pos(pos), _color(color), _intensity(1.f){}

		avt::Vector3 getPosition() {
			return _pos;
		}

		void setPosition(avt::Vector3 pos) {
			_pos = pos;
		}

		avt::Vector3 getColor() {
			return _color;
		}

		void setColor(avt::Vector3 color) {
			_color = color;
		}

		float getIntensity() {
			return _intensity;
		}

		void setIntensity(float intensity) {
			_intensity = intensity;
		}

		void updateLight(Shader& shader, const std::string& lightPositionUniform, const std::string& lightColorUniform) {
			shader.bind();
			shader.uploadUniformVec3(lightPositionUniform, _pos);
			shader.uploadUniformVec3(lightColorUniform, _intensity * _color);
			shader.unbind();
		}
	};
}