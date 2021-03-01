#pragma once

#include <vector>
#include <memory>
#include "avt_math.h"

namespace avt {
	class Renderable;
	class Shader;

	class SceneNode {
	private:
		SceneNode* _parent;
		std::vector<SceneNode*> _nodes;

		std::shared_ptr<Renderable> _rend;

		bool _dirty = false;
		Mat4 _localTranform;
		Mat4 _worldTransform = Mat4::identity();

		Vector3 _translation, _scale;
		Quaternion _rot;

		//mouse picking
		unsigned int _stencilIndex = 0; //0 = not selectable

	public:

		std::vector<SceneNode*>::iterator begin() { return _nodes.begin(); }
		std::vector<SceneNode*>::iterator end() { return _nodes.end(); }
		std::vector<SceneNode*>::const_iterator begin() const { return _nodes.begin(); }
		std::vector<SceneNode*>::const_iterator end() const { return _nodes.end(); }

	public:
		SceneNode(const std::shared_ptr<Renderable>& rend = nullptr)
			: _parent(nullptr), _translation(0, 0, 0), _scale(1.f, 1.f, 1.f), _rot({ 1.f,0,0 }, 0), _rend(rend)/*, _transform(Mat4::identity())*/ {}

		virtual ~SceneNode() {
			for (auto node : _nodes) {
				delete node;
			}
		}

		SceneNode* createNode(const std::shared_ptr<Renderable>& rend = nullptr) {
			SceneNode* node = new SceneNode(rend);
			_nodes.push_back(node);
			node->_parent = this;
			node->_dirty = true;
			return node;
		}

		SceneNode* addNode(SceneNode* node) {
			_nodes.push_back(node);
			node->_parent = this;
			node->_dirty = true;
			return node;
		}

		bool deleteNode(int index) {
			if (index < 0 || index >= _nodes.size()) return false;

			delete _nodes[index];
			_nodes.erase(_nodes.begin() + index);
			return true;
		}

		bool deleteNode(const SceneNode* node) {
			for (int i = 0; i < _nodes.size(); i++) {
				if (_nodes[i] == node) {
					delete _nodes[i];
					_nodes.erase(_nodes.begin() + i);
					return true;
				}
			}

			return false;
		}

		void deleteAll() {
			for (auto node : _nodes) {
				delete node;
			}
			_nodes.clear();
		}

		const std::vector<SceneNode*>& children() const {
			return _nodes;
		}

		void setRenderable(const std::shared_ptr<Renderable>& rend) {
			_rend = rend;
		}

		const std::shared_ptr<Renderable>& getRenderable() {
			return _rend;
		}


		/*void setTransform(const Mat4& transform) {
			_transform = transform;
		}

		const Mat4& getTransform() const {
			return _transform;
		}
		
		void applyTransform(const Mat4& transform) {
			_transform *= transform;
		}*/

		const Mat4& getWorldTransform() const {
			return _worldTransform;
		}

		void updateWorldFromParent() {
			if (!_parent) return;
			_worldTransform = _parent->_worldTransform * getTransform();
			_dirty = false;
		}

		bool dirty() const {
			return _dirty;
		}

		Mat4 getTransform() const {
			return Mat4::translation(_translation)
				* _rot.toMat()
				* Mat4::scale(_scale);
		}

		void setTranslation(const Vector3& v) {
			_translation = v;
			_dirty = true;
		}

		const Vector3& getTranslation() const {
			return _translation;
		}

		void translate(const Vector3& v) {
			_translation += v;
			_dirty = true;
		}

		void setScale(const Vector3& v) {
			_scale = v;
			_dirty = true;
		}

		const Vector3& getScale() const {
			return _scale;
		}

		void scale(const Vector3& v) {
			_scale *= v;
		}

		void rotateX(float rad) {
			_rot *= Quaternion({ 1.f,0,0 }, rad);
			_dirty = true;
		}

		void rotateY(float rad) {
			_rot *= Quaternion({ 0,1.f,0 }, rad);
			_dirty = true;
		}

		void rotateZ(float rad) {
			_rot *= Quaternion({ 0,0,1.f }, rad);
			_dirty = true;
		}

		void setRotation(const Quaternion& q) {
			_rot = q;
			_dirty = true;
		}

		const Quaternion& getRotation() const {
			return _rot;
		}

		void rotate(const Quaternion& q) {
			_rot *= q;
			_dirty = true;
		}

		SceneNode* getParent() {
			return _parent;
		}

		void setStencilIndex(unsigned int index) { //mouse picking
			_stencilIndex = index;
		}

		unsigned int getStencilIndex() { //mouse picking
			return _stencilIndex;
		}

	};

}