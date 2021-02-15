#pragma once

#include "SceneNode.h"

namespace avt {

	class Scene {
	private:
		SceneNode*_root;
	public:
		Scene() : _root(new SceneNode()) {}
		~Scene() {
			delete _root;
		}

		SceneNode* createNode() {
			return _root->createNode();
		}

		SceneNode* getRoot() const {
			return _root;
		}

		SceneNode* addNode(SceneNode* node) {
			return _root->addNode(node);
		}

		void setShader(Shader* shader) {
			_root->setShader(shader);
		}

		Shader* getShader() {
			return _root->getShader();
		}

	};

}