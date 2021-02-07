#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include "avt_math.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
//#include "UniformBuffer.h"
//#include "Shader.h"
//#include "Renderer.h"
#include "Camera.h"

#include "Texture.h"

namespace avt {

	struct Vertex{
		Vector3 position;
		Vector2 tex;
		Vector3 normal;
		Vector3 color;
	};

	class Mesh {
	private:

		VertexBuffer _vb;
		VertexArray _va;

		Texture* _texture = nullptr;

	protected:


	public:
		std::vector<Vertex> _meshData;

		Mesh() {}

		Mesh(const std::string& filename, const Vector3& baseColor = Vector3(1.f, 1.f, 1.f)) {
			addOBJ(filename, baseColor);
		}

		void addOBJ(const std::string& filename, const Vector3& baseColor = Vector3(1.f, 1.f, 1.f)) {
			auto data = loadOBJ(filename, baseColor);
			_meshData.insert(_meshData.end(), data.begin(), data.end());
		}

		void addFace(const Vertex& v1, const Vertex& v2, const Vertex& v3, bool computeFaceNormal = false);

		//  must be called before the first draw
		void setup();

		// call only after setup
		void updateBufferData();

		// saves memory if the mesh won't be modified again
		void clearLocalData() {
			_meshData.clear();
		}

		void setTexture(Texture* texture) {
			_texture = texture;
		}

		const Texture* texture() const {
			return _texture;
		}

		void colorAll(Vector3 color);
		void applyTransform(Mat4 mat);


		const VertexArray& va() const {
			return _va;
		}

		const VertexBuffer& vb() const {
			return _vb;
		}

		// produces sharp meshes
		void computeFaceNormals();
		// produces smooth meshes
		void computeVertexNormals(bool weighted = true);
		// produces smooth transitions only when the angle between the face normals is below the threshold
		void computeMixedNormals(float threshold, bool weighted = true);


		static std::vector<Vertex> loadOBJ(const std::string& filename, const Vector3& baseColor = Vector3(1.f, 1.f, 1.f));

	private:

		static void parseLine(const std::string& line, std::vector<Vertex>& data, std::vector<Vector3>& vertices, std::vector<Vector2>& textures, std::vector<Vector3>& normals, const Vector3& baseColor, Vector3& color);
		static void parseVertex(std::stringstream& sin, std::vector<Vector3>& vertices);
		static void parseTexture(std::stringstream& sin, std::vector<Vector2>& textures);
		static void parseNormal(std::stringstream& sin, std::vector<Vector3>& normals);
		static void parseFace(std::stringstream& sin, std::vector<Vertex>& data, const std::vector<Vector3>& vertices, const std::vector<Vector2>& textures, const std::vector<Vector3>& normals, const Vector3& color);
		static Vector3 parseMaterial(std::stringstream& sin, const Vector3& baseColor);

	};
}