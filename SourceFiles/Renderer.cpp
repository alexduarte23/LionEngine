#include "../HeaderFiles/Renderer.h"


#include "../HeaderFiles/avt_math.h"
#include "../HeaderFiles/Mesh.h"
#include "../HeaderFiles/StencilPicker.h"

#include "../HeaderFiles/UniformBuffer.h"
#include "../HeaderFiles/VertexArray.h"
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Camera.h"

#include "../HeaderFiles/Scene.h"
#include "../HeaderFiles/SceneNode.h"

namespace avt {

	/*void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, UniformBuffer& ub, Shader& shader, Camera* camera) {
		va.bind();
		ib.bind();
		ub.bind();
		shader.bind();

		ub.fill({ camera->viewMatrix(), camera->projMatrix()});

		shader.uploadUniformMat4(MODEL_MATRIX, Mat4::identity());
		glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_BYTE, (GLvoid*)0);

		shader.unbind();
		va.unbind();
		ib.unbind();
		ub.unbind();
	}*/




	void Renderer::draw(const Scene& scene, Camera* camera) {
		if (_autoClear) clear();

		auto& ub = camera->getUBO();
		ub->bind();
		ub->upload({ camera->viewMatrix(), camera->projMatrix() });

		drawNode(scene.getRoot(), Mat4::identity());

		ub->unbind();
	}

	void Renderer::drawNode(SceneNode* node, const Mat4& worldMatrix) {
		node->beforeDraw();
		auto newWorldMat = worldMatrix * node->getTransform();

		if (node->getRenderable()) drawRenderable(node->getRenderable(), newWorldMat);

		for (auto childNode : *node) {
			drawNode(childNode, newWorldMat);
		}
		node->afterDraw();
	}

	void Renderer::drawRenderable(const std::shared_ptr<Renderable>& rend, const Mat4& worldMatrix) {
		auto& mesh = rend->mesh();
		auto& shader = rend->shader();

		if (mesh->autoBufferUpdate()) mesh->updateBufferData();
		
		auto& va = mesh->va();
		if (!va || !shader) return;

		va->bind();
		shader->bind();
		shader->uploadUniformMat4(MODEL_MATRIX, worldMatrix);

		glDrawArrays(getGLdrawMode(rend->drawMode()), 0, mesh->vertexCount());

		shader->unbind();
		va->unbind();
	}


	void Renderer::clear() const {
		if (_clearStencil) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}