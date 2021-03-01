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

	void Renderer::begin(Camera* camera) {
		if (_autoClear) clear();

		auto& ub = camera->getUBO();
		ub->bind();
		ub->upload({ camera->viewMatrix(), camera->projMatrix() });

		_activeCam = camera;
	}

	void Renderer::end() {
		_activeCam->getUBO()->unbind();
		_activeCam = nullptr;
		_subs.clear();
	}

	void Renderer::traverse(SceneNode* node, bool dirty) {
		//auto newWorldMat = worldMatrix * node->getTransform();
		dirty = dirty || node->dirty();
		if (dirty) node->updateWorldFromParent();

		if (node->getRenderable()) submit(node->getRenderable(), node->getWorldTransform());

		for (auto childNode : *node) {
			traverse(childNode, dirty);
		}
	}

	void Renderer::submit(const std::shared_ptr<Renderable>& rend, const Mat4& worldMatrix) {
		auto& mesh = rend->mesh();
		auto& shader = rend->shader();

		if (mesh && mesh->autoBufferUpdate()) mesh->updateBufferData();
		if (!shader || !mesh || !mesh->va()) return;

		auto it = _subs.find(shader);
		if (it == _subs.end()) { // new shader
			_subs.insert({ shader, {{mesh, {worldMatrix}}} });
		} else { // seen shader
			auto it2 = it->second.find(mesh);
			if (it2 == it->second.end()) { // new mesh
				it->second.insert({ mesh, {worldMatrix} });
			} else { // seen mesh
				it2->second.push_back(worldMatrix);
			}
		}
	}

	void Renderer::draw() {
		for (auto& shaderGroup : _subs) {
			auto& shader = shaderGroup.first;
			shader->bind();
			//std::cout << "shader: " << shader << std::endl;
			for (auto& meshGroup : shaderGroup.second) {
				auto& mesh = meshGroup.first;
				mesh->va()->bind();
				//std::cout << "mesh: " << mesh << std::endl;
				for (auto& transform : meshGroup.second) {
					shader->uploadModelMatrix(transform);
					glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount());
				}
				//std::cout << meshGroup.second.size() << " tranforms" << std::endl;
				mesh->va()->unbind();
			}
			shader->unbind();
		}
	}



	void Renderer::draw(const Scene& scene, Camera* camera) {

		//begin(camera);
		//traverse(scene.getRoot(), false);
		//draw();
		//end();
		//return;

		if (_autoClear) clear();

		auto& ub = camera->getUBO();
		ub->bind();
		ub->upload({ camera->viewMatrix(), camera->projMatrix() });

		drawNode(scene.getRoot(), false);

		ub->unbind();
	}

	void Renderer::drawNode(SceneNode* node, bool dirty) {
		//auto newWorldMat = worldMatrix * node->getTransform();
		dirty = dirty || node->dirty();
		if (dirty) node->updateWorldFromParent();

		if (node->getRenderable()) drawRenderable(node->getRenderable(), node->getWorldTransform());

		for (auto childNode : *node) {
			drawNode(childNode, dirty);
		}
	}

	void Renderer::drawRenderable(const std::shared_ptr<Renderable>& rend, const Mat4& worldMatrix) {
		auto& mesh = rend->mesh();
		auto& shader = rend->shader();

		if (mesh->autoBufferUpdate()) mesh->updateBufferData();
		
		auto& va = mesh->va();
		if (!va || !shader) return;

		va->bind();
		shader->bind();
		shader->uploadModelMatrix(worldMatrix);

		glDrawArrays(getGLdrawMode(rend->drawMode()), 0, mesh->vertexCount());

		shader->unbind();
		va->unbind();
	}


	void Renderer::clear() const {
		if (_clearStencil) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}