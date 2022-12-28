#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"

using json = nlohmann::json;

class Model
{
public:
	// Loads in a model from a file and stores file info
	Model(const char* file, unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

private:
	// gtlf file data
	const char* file;
	std::vector<unsigned char> bin;
	json gtlfJSON;

	// Holds number of instances (if 1 the mesh will be rendered normally)
	unsigned int instancing;

	// meshes and transforms
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;
	std::vector<glm::mat4> instanceMatrix;

	std::vector<std::string> loadedTexturePaths;
	std::vector<Texture> loadedTextures;

	// Load Mesh by index
	void loadMesh(unsigned int indMesh);

	// Traverse node and children recursively set object data members
	void traverseNode(unsigned int nextNode, glm::mat4 IntialTransformMatrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> LoadBin();

	// Interpret binary data into values
	std::vector<float> floatsFromBin(json accessor);
	std::vector<GLuint> indicesFromBin(json accessor);
	std::vector<Texture> texturesFromBin();

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif