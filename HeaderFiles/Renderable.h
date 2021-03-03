#pragma once

#include <memory>

namespace avt {
	class Mesh;
	class Material;

	enum class DrawMode {
		Triangles, Strip, Fan, Point, Line, Path, ClosedPath
	};

	class Renderable {
	protected:
		DrawMode _mode;
		std::shared_ptr<Mesh> _mesh;
		std::shared_ptr<Material> _material;

		Renderable() : _mode(DrawMode::Triangles) {}

		Renderable(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, DrawMode mode)
			: _mesh(mesh), _material(material), _mode(mode) {}

	public:
		virtual ~Renderable() {}

		const std::shared_ptr<Mesh>& mesh() const { return _mesh; }
		const std::shared_ptr<Material>& material() const { return _material; }
		DrawMode drawMode() const { return _mode; }

	};

}