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

		SceneNode* createNode(const std::shared_ptr<Renderable>& rend = nullptr) {
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