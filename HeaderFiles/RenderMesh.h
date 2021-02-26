#pragma once

#include <memory>
#include "Renderable.h"

namespace avt {

	class RenderMesh : public Renderable {
	private:
	public:
		RenderMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader)
			: Renderable(mesh, shader, DrawMode::Triangles) {}

		~RenderMesh() {}


	};


}