#pragma once

#include <memory>

namespace avt {
	class Mesh;
	class Shader;

	enum class DrawMode {
		Triangles, Strip, Fan, Point, Line, Path, ClosedPath
	};

	class Renderable {
	protected:
		DrawMode _mode;
		std::shared_ptr<Mesh> _mesh;
		std::shared_ptr<Shader> _shader;

		Renderable() : _mode(DrawMode::Triangles) {}

		Renderable(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader, DrawMode mode)
			: _mesh(mesh), _shader(shader), _mode(mode) {}

	public:
		virtual ~Renderable() {}

		const std::shared_ptr<Mesh>& mesh() const { return _mesh; }
		const std::shared_ptr<Shader>& shader() const { return _shader; }
		DrawMode drawMode() const { return _mode; }

	};

}