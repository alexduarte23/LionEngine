#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include "Renderable.h"

namespace avt {

	class Camera;
	class Scene;
	class Shader;
	class SceneNode;
	class Mat4;

	static inline GLenum getGLdrawMode(DrawMode mode) {
		switch (mode) {
		case DrawMode::Triangles:	return GL_TRIANGLES;
		case DrawMode::Strip:		return GL_TRIANGLE_STRIP;
		case DrawMode::Fan:			return GL_TRIANGLE_FAN;
		case DrawMode::Point:		return GL_POINTS;
		case DrawMode::Line:		return GL_LINES;
		case DrawMode::Path:		return GL_LINE_STRIP;
		case DrawMode::ClosedPath:	return GL_LINE_LOOP;
		default:					return GL_TRIANGLES;
		}
	}

	class Renderer {
	private:

		bool _autoClear = true;
		bool _clearStencil = true;

		Camera* _activeCam = nullptr;
		std::unordered_map<std::shared_ptr<Material>, std::unordered_map<std::shared_ptr<Mesh>, std::vector<Mat4>>> _subs;

		void begin(Camera* camera);
		void end();
		void traverse(SceneNode* node, bool dirty);
		void submit(const std::shared_ptr<Renderable>& rend, const Mat4& worldMatrix);
		void draw();

		void drawNode(SceneNode* node, bool dirty);
		void drawRenderable(const std::shared_ptr<Renderable>& rend, const Mat4& worldMatrix);

	public:
		Renderer() {}
		~Renderer() {}

		void draw(const Scene& scene, Camera* camera);


		void clear() const;

		void autoClear(bool clear = true) {
			_autoClear = clear;
		}

		void setStencilClear(bool clear = true) {
			_clearStencil = clear;
		}
	};

}