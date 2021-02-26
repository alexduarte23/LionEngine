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

		SceneNode* createNode(Renderable* rend = nullptr) {
			return _root->createNode(rend);
		}

		SceneNode* getRoot() const {
			return _root;
		}

		SceneNode* addNode(SceneNode* node) {
			return _root->addNode(node);
		}

	};

}