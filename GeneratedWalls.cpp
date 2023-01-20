#include "GeneratedWalls.h"

#include <iostream>

GeneratedWalls::GeneratedWalls(
	Camera* camera,
	const char* modelFile,
	glm::vec3 initTranslation,
	glm::vec3 initScale,
	glm::quat initRotation) :
	camera(camera),
	GameObject(
		modelFile,
		initTranslation,
		initScale,
		initRotation)
{
	SetWallDimensions();
	UpdateWorldVertices();
}

void GeneratedWalls::Draw(Shader& shader)
{
	glDepthFunc(GL_LESS);

	int sideClusters = (visibleClustersToEdge * 2 + 1);
	for (int clusterIndex = 0; clusterIndex < pow(sideClusters, 3); ++clusterIndex)
	{
		glm::vec3 clusterTranslation = GetClusterTranslation(clusterIndex, visibleClustersToEdge);

		for (int wallIndex = 0; wallIndex < wallsPerCluster; ++wallIndex)
		{
			glm::vec3 wallTranslation = GetWallTranslation(wallIndex, clusterIndex);
			glm::vec3 modelTranslation = clusterTranslation + wallTranslation + translation;

			//2 * acos(0.0) is pi
			glm::quat wallRotation = glm::vec3(
				0.0f,
				0.0f,
				((clusterIndex + wallIndex) % 2) * acos(0.0));
			glm::quat modelRotation = wallRotation + rotation;

			model.Draw(
				shader,
				modelTranslation,
				modelRotation,
				scale);
		}
	}
}

void GeneratedWalls::Update()
{
	// TODO:	ADD 3d cluster extension for solid and visible walls here
	//			can be copied from GeneratedGround
}

void GeneratedWalls::SetWallDimensions()
{

	// converting transformations to mat4 for combination
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	std::vector<Mesh>& meshes = model.meshes;
	std::vector<glm::mat4>& transforms = model.meshTransforms;

	//for width calculation
	glm::vec3 maxBounds = meshes[0].vertices[0].position;
	glm::vec3 minBounds = meshes[0].vertices[0].position;

	for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		Mesh& mesh = meshes[meshIndex];
		glm::mat4& meshTransform = transforms[meshIndex];

		// setting offest in case of multiple meshes
		GLuint indexOffset = (GLuint)vertexWorldPositions.size();

		std::vector<Vertex>& vertices = mesh.vertices;

		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			// Apply transformation to get world position
			// TODO: There might be a better way to do this (copied from shader)
			glm::vec3 vertexWorldPosition(meshTransform * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));

			for (int axes = 0; axes < 3; ++axes)
			{
				minBounds[axes] = 
					vertexWorldPosition[axes] < minBounds[axes] ?
					vertexWorldPosition[axes] :
					minBounds[axes];

				maxBounds[axes] =
					vertexWorldPosition[axes] > maxBounds[axes] ?
					vertexWorldPosition[axes] :
					maxBounds[axes];
			}
		}
	}
	wallDimensions = maxBounds - minBounds;
}

glm::vec3 GeneratedWalls::GetClusterTranslation(int clusterIndex, int numToEdge)
{
	glm::vec3 translationOffsets;
	int sideClusters = (numToEdge * 2 + 1);
	for (int axis = 0; axis < 3; ++axis)
	{
		float startingBound = -clusterDimensions[axis] * numToEdge;
		int sideIndex = clusterIndex / (int)(pow(sideClusters, axis)) % sideClusters;
		translationOffsets[axis] = startingBound + sideIndex * clusterDimensions[axis];
	}

	return translation - translationOffsets;

}

// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key

glm::vec3 GeneratedWalls::GetWallTranslation(int wallIndex, int clusterIndex)
{
	int seed = (wallIndex + 1) * (clusterIndex + 1);
	glm::vec3 wallTranslation;
	for (int axes = 0; axes < 3 ; ++axes)
	{
		unsigned int x = (axes + 1) * seed;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		wallTranslation[axes] = x % (int)clusterDimensions[axes];
	}
		
	return wallTranslation;
}

void GeneratedWalls::UpdateWorldVertices()
{
	triangleWorldPositions.clear();
	vertexWorldPositions.clear();
	triangleWorldNormals.clear();
	vertexWorldNormals.clear();

	std::vector<Mesh>& meshes = model.meshes;
	std::vector<glm::mat4>& transforms = model.meshTransforms;

	int sideClusters = (solidClustersToEdge * 2 + 1);
	for (int clusterIndex = 0; clusterIndex < pow(sideClusters, 3); ++clusterIndex)
	{
		glm::vec3 clusterTranslation = GetClusterTranslation(clusterIndex, solidClustersToEdge);

		for (int wallIndex = 0; wallIndex < wallsPerCluster; ++wallIndex)
		{
			glm::vec3 wallTranslation = GetWallTranslation(wallIndex, clusterIndex);
			glm::vec3 modelTranslation = clusterTranslation + wallTranslation + translation;

			//2 * acos(0.0) is pi
			glm::quat wallRotation = glm::vec3(
				0.0f,
				0.0f,
				((clusterIndex + wallIndex) % 2) * acos(0.0));
			glm::quat modelRotation = wallRotation + rotation;

			// converting transformations to mat4 for combination
			glm::mat4 translationMatrix = glm::mat4(1.0f);
			glm::mat4 rotationMatrix = glm::mat4(1.0f);
			glm::mat4 scaleMatrix = glm::mat4(1.0f);

			translationMatrix = glm::translate(translationMatrix, modelTranslation);
			rotationMatrix = glm::mat4_cast(modelRotation);
			scaleMatrix = glm::scale(scaleMatrix, scale);

			for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
			{
				Mesh& mesh = meshes[meshIndex];
				glm::mat4& meshTransform = transforms[meshIndex];

				// setting offest in case of multiple meshes
				GLuint indexOffset = (GLuint)vertexWorldPositions.size();

				std::vector<Vertex>& vertices = mesh.vertices;

				for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
				{
					Vertex& vertex = vertices[vertexIndex];

					// Apply transformation to get world position
					// TODO: There might be a better way to do this (copied from shader)
					glm::vec3 vertexWorldPosition(meshTransform * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
					glm::vec3 vertexWorldNormal(normalize(rotationMatrix * scaleMatrix * glm::vec4(vertex.normal, 1.0f)));

					vertexWorldPositions.push_back(vertexWorldPosition);
					vertexWorldNormals.push_back(normalize(vertexWorldNormal));
					
				}

				// collecting vertices in triangle order
				std::vector<GLuint>& indices = meshes[meshIndex].indices;

				for (int i = 0; i < indices.size(); ++i)
				{
					GLuint& vertexIndice = indices[i];
					glm::vec3& worldPosition = vertexWorldPositions[vertexIndice + indexOffset];
					glm::vec3& worldNormal = vertexWorldNormals[vertexIndice + indexOffset];
					triangleWorldPositions.push_back(worldPosition + normalize(worldNormal));
					triangleWorldNormals.push_back(worldNormal);
				}

				for (int i = 0; i < indices.size(); ++i)
				{
					GLuint& vertexIndice = indices[i];
					glm::vec3& worldPosition = vertexWorldPositions[vertexIndice + indexOffset];
					glm::vec3& worldNormal = vertexWorldNormals[vertexIndice + indexOffset];
					triangleWorldPositions.push_back(worldPosition - normalize(worldNormal));
					triangleWorldNormals.push_back(-worldNormal);
				}
			}
		}

	}
	validWorldPositions = true;
}
