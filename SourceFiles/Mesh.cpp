#include "../HeaderFiles/Mesh.h"

#include <set>

namespace avt {
	void Mesh::colorAll(Vector3 color) {
		for (auto& v : _meshData) {
			v.color = color;
		}
		_dirty = true;
	}

	void Mesh::applyTransform(Mat4 mat) {
		for (auto& v : _meshData) {
			v.position = (mat * v.position.to4D()).to3D();
		}
		_dirty = true;
	}

	void Mesh::setup() {
		VertexBufferLayout layout({
			{ShaderDataType::VEC3, "position"},
			{ShaderDataType::VEC2, "texCoord"},
			{ShaderDataType::VEC3, "normal"},
			{ShaderDataType::VEC3, "color"}
		});
		//_vb = std::make_shared<VertexBuffer>(_meshData.data(), _meshData.size() * sizeof(Vertex), layout);
		_vb = std::make_shared<VertexBuffer>(_meshData, layout);
		_vertexNum = static_cast<int>(_meshData.size());

		_va = std::make_shared<VertexArray>();
		_va->addVertexBuffer(_vb);

		_va->unbind();
		_vb->unbind();
	}

	void Mesh::updateBufferData() {
		if (!_dirty) return;
		if (!_va.get()) {
			setup();
			_dirty = false;
			return;
		}

		_vb->upload(_meshData);
		_vb->unbind();

		_vertexNum = static_cast<int>(_meshData.size());
		_dirty = false;
	}

	void Mesh::addFace(const Vertex& v1, const Vertex& v2, const Vertex& v3, bool computeFaceNormal) {
		_meshData.push_back(v1);
		_meshData.push_back(v2);
		_meshData.push_back(v3);

		if (computeFaceNormal) {
			Vector3 normal = (v1.position - v2.position).cross(v2.position - v3.position).normalized();
			size_t size = _meshData.size();
			_meshData[size - 1].normal = normal;
			_meshData[size - 2].normal = normal;
			_meshData[size - 3].normal = normal;
		}
		_dirty = true;
	}

	void Mesh::computeFaceNormals() {
		for (size_t i = 0; i < _meshData.size(); i += 3) {
			Vertex& v1 = _meshData[i];
			Vertex& v2 = _meshData[i + 1];
			Vertex& v3 = _meshData[i + 2];

			Vector3 normal = (v1.position-v2.position).cross(v2.position - v3.position).normalized();

			v1.normal = normal;
			v2.normal = normal;
			v3.normal = normal;
		}
		_dirty = true;
	}

	void Mesh::computeVertexNormals(bool weighted) {
		std::vector<Vertex*> current;
		const Vector3 zero;

		for (auto& v : _meshData) {
			v.normal = Vector3();
		}

		for (size_t i = 0; i < _meshData.size(); i++) {
			if (_meshData[i].normal != zero) continue;

			Vector3 normal;
			for (size_t j = i; j < _meshData.size(); j++) {
				if (_meshData[i].position != _meshData[j].position) continue;

				Vertex& v1 = _meshData[j - j%3];
				Vertex& v2 = _meshData[j - j%3 + 1];
				Vertex& v3 = _meshData[j - j%3 + 2];

				Vector3 fn = (v1.position - v2.position).cross(v2.position - v3.position);
				if (!weighted) fn.normalize();
				normal += fn;

				current.push_back(&_meshData[j]);
			}
			normal.normalize();
			for (auto v : current) {
				v->normal = normal;
			}

			current.clear();
		}
		_dirty = true;
	}

	void Mesh::computeMixedNormals(float threshold, bool weighted) {
		std::vector<Vertex*> current;
		const Vector3 zero;

		for (auto& v : _meshData) {
			v.normal = Vector3();
		}

		for (size_t i = 0; i < _meshData.size(); i++) {
			if (_meshData[i].normal != zero) continue;

			Vertex& v1 = _meshData[i - i % 3];
			Vertex& v2 = _meshData[i - i % 3 + 1];
			Vertex& v3 = _meshData[i - i % 3 + 2];

			Vector3 baseNormal = (v1.position - v2.position).cross(v2.position - v3.position);
			Vector3 normal = baseNormal;
			if (!weighted) normal.normalize();
			current.push_back(&_meshData[i]);

			for (size_t j = i - i%3 + 3; j < _meshData.size(); j++) {
				if (_meshData[i].position != _meshData[j].position || _meshData[j].normal != zero) continue;

				Vertex& v1 = _meshData[j - j % 3];
				Vertex& v2 = _meshData[j - j % 3 + 1];
				Vertex& v3 = _meshData[j - j % 3 + 2];

				Vector3 fn = (v1.position - v2.position).cross(v2.position - v3.position);

				if (fn.angleTo(baseNormal) > threshold) continue;

				if (!weighted) fn.normalize();
				normal += fn;

				current.push_back(&_meshData[j]);
			}
			normal.normalize();
			for (auto v : current) {
				v->normal = normal;
			}

			current.clear();
		}
		_dirty = true;
	}

	// OBJ LOADING

	std::vector<Vertex> Mesh::loadOBJ(const std::string& filename, const Vector3& baseColor) {
		std::vector<Vertex> data;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> textures;

		Vector3 color = baseColor;

		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			parseLine(line, data, vertices, textures, normals, baseColor, color);
		}

		return data;
	}

	void Mesh::parseLine(const std::string& line, std::vector<Vertex>& data, std::vector<Vector3>& vertices, std::vector<Vector2>& textures, std::vector<Vector3>& normals, const Vector3& baseColor, Vector3& color) {
		std::stringstream sline(line);
		std::string s;

		sline >> s;
		if (s.compare("v") == 0) parseVertex(sline, vertices);
		else if (s.compare("vt") == 0) parseTexture(sline, textures);
		else if (s.compare("vn") == 0) parseNormal(sline, normals);
		else if (s.compare("f") == 0) parseFace(sline, data, vertices, textures, normals, color);
		else if (s.compare("usemtl") == 0) color = parseMaterial(sline, baseColor);
	}

	void Mesh::parseVertex(std::stringstream& sin, std::vector<Vector3>& vertices) {
		Vector3 v;
		sin >> v;
		vertices.push_back(v);
	}

	void Mesh::parseTexture(std::stringstream& sin, std::vector<Vector2>& textures) {
		Vector2 t;
		sin >> t;
		textures.push_back(t);
	}

	void Mesh::parseNormal(std::stringstream& sin, std::vector<Vector3>& normals) {
		Vector3 n;
		sin >> n;
		normals.push_back(n);
	}

	void Mesh::parseFace(std::stringstream& sin, std::vector<Vertex>& data, const std::vector<Vector3>& vertices, const std::vector<Vector2>& textures, const std::vector<Vector3>& normals, const Vector3& color) {
		std::string vertexString, token;
		
		for (size_t i = 0; i < 3; i++) {
			Vertex v;
			sin >> vertexString;
			std::stringstream sinV(vertexString);

			std::getline(sinV, token, '/');
			v.position = vertices[std::stoll(token) - 1];

			std::getline(sinV, token, '/');
			v.tex = token.size() ? textures[std::stoll(token) - 1] : Vector2(0,0);

			std::getline(sinV, token, '/');
			v.normal = token.size() ? normals[std::stoll(token) - 1] : Vector3(0, 0, 0);

			v.color = color;

			data.push_back(v);
			
		}
	}

	Vector3 Mesh::parseMaterial(std::stringstream& sin, const Vector3& baseColor) {
		std::string token;
		Vector3 color;

		try {
			std::getline(sin, token, ',');
			color.x = std::stof(token);

			std::getline(sin, token, ',');
			color.y = std::stof(token);

			std::getline(sin, token, ',');
			color.z = std::stof(token);

			return color;
		}
		catch (...) {
			return baseColor;
		}
	}

}