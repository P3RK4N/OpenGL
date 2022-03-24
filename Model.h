#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<unordered_map>

#include<json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
	public:
		Model(const char* file);
		void Draw(Shader& shader, Camera& camera);

	private:
		const char* file;
		std::vector<unsigned char> data;
		json JSON;

		std::vector<Mesh> meshes;
		std::vector<glm::vec3> translationsMeshes;
		std::vector<glm::quat> rotationsMeshes;
		std::vector<glm::vec3> scalesMeshes;
		std::vector<glm::mat4> matricesMeshes;

		std::unordered_map<std::string, unsigned int> loadedTexName;
		std::vector<Texture> loadedTextures;

		std::vector<unsigned char>& getData();
		std::vector<float>&	getFloats(json accessor);
		std::vector<GLuint>& getIndices(json accessor);
		std::vector<Texture>& getTextures();

		std::vector<Vertex>& assembleVertices
		(
			std::vector<glm::vec3> position,
			std::vector<glm::vec3> normal,
			std::vector<glm::vec2> uv
		);

		void loadMesh(unsigned int indMesh);

		void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

		std::vector<glm::vec2>& groupFloatsVec2(std::vector<float>& floatVec);
		std::vector<glm::vec3>& groupFloatsVec3(std::vector<float>& floatVec);
		std::vector<glm::vec4>& groupFloatsVec4(std::vector<float>& floatVec);
};

#endif