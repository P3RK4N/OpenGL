#include "Model.h"

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	Model::JSON = json::parse(text);
	data = getData();
}

std::vector<unsigned char>& Model::getData()
{
	std::string bytesText;
	std::string uri = Model::JSON["buffers"][0]["uri"];
	std::string fileStr = std::string(Model::file);
	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDir + uri).c_str());
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float>& Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = Model::JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Invalid type");

	unsigned int beginData = accByteOffset + byteOffset;
	unsigned int lengthData = count * 4 * numPerVert;

	for (unsigned int i = beginData; i < beginData + lengthData;)
	{
		unsigned char bytes[] = { Model::data[i++], Model::data[i++], Model::data[i++], Model::data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint>& Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = Model::JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];
	
	unsigned int beginData = byteOffset + accByteOffset;
	unsigned int lengthData = count * 4;

	if (componentType == 5125)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture>& Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];

		if (Model::loadedTexName.find(texPath) != Model::loadedTexName.end())
		{
			textures.push_back(Model::loadedTextures[Model::loadedTexName[texPath]]);
			continue;
		}

		if (texPath.find("baseColor") != std::string::npos)
		{
			Texture diffuse((fileDir + texPath).c_str(), DIFFUSE, GL_NEAREST, GL_REPEAT);
			textures.push_back(diffuse);
			Model::loadedTextures.push_back(diffuse);
			Model::loadedTexName[texPath] = Model::loadedTextures.size() - 1;
		}
		else if (texPath.find("metallicRoughness") != std::string::npos)
		{
			Texture specular = Texture((fileDir + texPath).c_str(), SPECULAR, GL_NEAREST, GL_REPEAT);
			textures.push_back(specular);
			Model::loadedTextures.push_back(specular);
			Model::loadedTexName[texPath] = Model::loadedTextures.size() - 1;
		}
	}

	return textures;
}

std::vector<Vertex>& Model::assembleVertices(std::vector<glm::vec3> position, std::vector<glm::vec3> normal, std::vector<glm::vec2> uv)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < position.size(); i++)
	{
		vertices.push_back(Vertex{ position[i], normal[i], glm::vec3(0.0f), uv[i]});
	}
	return vertices;
}

void Model::loadMesh(unsigned int indMesh)
{
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int uvAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	std::vector<float> posVec = Model::getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = Model::groupFloatsVec3(posVec);

	std::vector<float> normalVec = Model::getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = Model::groupFloatsVec3(normalVec);

	std::vector<float> uvVec = Model::getFloats(JSON["accessors"][uvAccInd]);
	std::vector<glm::vec2> UVs = Model::groupFloatsVec2(uvVec);

	std::vector<Vertex> vertices = Model::assembleVertices(positions, normals, UVs);
	std::vector<GLuint> indices = Model::getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = Model::getTextures();

	meshes.push_back(Mesh(vertices, indices, textures));
}

//void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f))
//{
//
//}

std::vector<glm::vec2>& Model::groupFloatsVec2(std::vector<float> &floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size();)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec3>& Model::groupFloatsVec3(std::vector<float> &floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size();)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec4>& Model::groupFloatsVec4(std::vector<float> &floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size();)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}