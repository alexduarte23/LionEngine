#pragma once


namespace avt {

	class Shader;
	class Camera;
	class Scene;
	class Mesh;
	class SceneNode;
	class Mat4;

	class Renderer {
	private:

		bool _autoClear = true;
		bool _clearStencil = true;


	public:
		//Renderer (const void* data, GLuint count);
		Renderer() {}
		~Renderer() {}

		//void draw(const VertexArray& va, const IndexBuffer& ib, UniformBuffer& ub, Shader& shader, Camera* camera);
		void draw(const Scene& scene, Camera* camera);

		void drawNode(SceneNode* node, const Mat4& worldMatrix);
		void drawMesh(Mesh* mesh, const Mat4& worldMatrix);

		void clear() const;

		void autoClear(bool clear = true) {
			_autoClear = clear;
		}

		void setStencilClear(bool clear = true) {
			_clearStencil = clear;
		}
	};

}