#pragma once

#include <GL/glew.h>
#include "Renderable.h"

namespace avt {

	class Camera;
	class Scene;
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


	public:
		Renderer() {}
		~Renderer() {}

		void draw(const Scene& scene, Camera* camera);

		void drawNode(SceneNode* node, const Mat4& worldMatrix);
		void drawRenderable(Renderable* rend, const Mat4& worldMatrix);

		void clear() const;

		void autoClear(bool clear = true) {
			_autoClear = clear;
		}

		void setStencilClear(bool clear = true) {
			_clearStencil = clear;
		}
	};

}