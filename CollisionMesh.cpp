#include "CollisionMesh.h"

#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream> // REMOVE

void CollisionMesh::SetMesh(Model  model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	std::vector<Mesh>& meshes = model.GetMeshes();
	std::vector<glm::mat4>& meshTransforms = model.GetMeshTransforms();

	for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		// collecting vertices
		glm::mat4& meshPos = meshTransforms[meshIndex];
		std::vector<Vertex>& vertices = meshes[meshIndex].vertices;
		int startingVertexIndex = vertexPositions.size();
		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			glm::vec3 vertexWorldPosition(meshPos * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			glm::vec3 vertexWorldNormal(rotationMatrix * glm::vec4(vertex.normal, 1.0f));
			vertexPositions.push_back(vertexWorldPosition);
			vertexNormals.push_back(normalize(vertexWorldNormal));
		}
		// collecting indices
		std::vector<GLuint>& indices = meshes[meshIndex].indices;
		for (int i = 0; i < indices.size(); ++i)
		{
			GLuint& indice = indices[i];
			vertexIndices.push_back(indice + startingVertexIndex);
		}
	}
}

float CollisionMesh::signed_tetra_volume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	return glm::sign(glm::dot(glm::cross(b - a, c - a), d - a) / 6.0);
}

glm::vec3 CollisionMesh::GetAdjustedDestination(glm::vec3 start, glm::vec3 destination)
{
	for (int triangleIndex = 0; triangleIndex < vertexIndices.size(); triangleIndex += 3)
	{
		GLuint vertexIndiceA = vertexIndices[triangleIndex];
		GLuint vertexIndiceB = vertexIndices[triangleIndex + 1];
		GLuint vertexIndiceC = vertexIndices[triangleIndex + 2];


		glm::vec3 vertexNormalA = vertexNormals[vertexIndiceA];
		glm::vec3 vertexNormalB = vertexNormals[vertexIndiceB];
		glm::vec3 vertexNormalC = vertexNormals[vertexIndiceC];

		glm::vec3 vertexPositionA = vertexPositions[vertexIndiceA] + 0.1f * normalize(vertexNormalA);
		glm::vec3 vertexPositionB = vertexPositions[vertexIndiceB] + 0.1f * normalize(vertexNormalB);
		glm::vec3 vertexPositionC = vertexPositions[vertexIndiceC] + 0.1f * normalize(vertexNormalC);

		glm::vec3 triangleNormal = (vertexNormalA + vertexNormalB + vertexNormalC) / 3.0f;
		glm::vec3 triangleCenter = (vertexPositionA + vertexPositionB + vertexPositionC) / 3.0f;

		//formula modified from: https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d

		float s1 = signed_tetra_volume(start, vertexPositionA, vertexPositionB, vertexPositionC);
		float s2 = signed_tetra_volume(destination, vertexPositionA, vertexPositionB, vertexPositionC);

		if (s1 != s2)
		{
			float s3 = signed_tetra_volume(start, destination, vertexPositionA, vertexPositionB);
			float s4 = signed_tetra_volume(start, destination, vertexPositionB, vertexPositionC);
			float s5 = signed_tetra_volume(start, destination, vertexPositionC, vertexPositionA);
			if (s3 == s4 && s4 == s5)
			{
				float t = glm::dot(triangleCenter - start, triangleNormal) / glm::dot(destination - start, triangleNormal);
				return start + t * (destination - start);

			}
		}
	}
	return destination;
}

/*
		//check for plane treversal
		if (glm::dot(vertexPositionA - start, triangleNormal) < 0 &&
			glm::dot(vertexPositionA - destination, triangleNormal) > 0)
		{
			// get position components orthagnonal to translation to check if triangle is hit
			// 
			//         destination
			//			  /
			//         C /                 C
			//        /\/                 /|
			//       /  \      ----->    /.|   
			//      /_/__\              /__|
			//     A /    B            A   B
			//      /
			//     start
			//
			glm::vec3 translation = destination - start;
			glm::vec3 TranslationSpaceComponentA = glm::proj(vertexPositionA, translation);
			glm::vec3 TranslationSpaceComponentB = glm::proj(vertexPositionB, translation);
			glm::vec3 TranslationSpaceComponentC = glm::proj(vertexPositionC, translation);
			glm::vec3 TranslationSpaceComponentStart = glm::proj(start, translation);
			glm::vec3 TranslationSpaceVertexPositionA = vertexPositionA - TranslationSpaceComponentA;
			glm::vec3 TranslationSpaceVertexPositionB = vertexPositionB - TranslationSpaceComponentB;
			glm::vec3 TranslationSpaceVertexPositionC = vertexPositionC - TranslationSpaceComponentC;
			glm::vec3 passthrough = start - TranslationSpaceComponentStart; // could be destination (should be same)

			// check angles to determine if passthrough point is within triangle 

			glm::vec3 ABTransSpace = TranslationSpaceVertexPositionB - TranslationSpaceVertexPositionA;
			glm::vec3 ACTransSpace = TranslationSpaceVertexPositionC - TranslationSpaceVertexPositionA;
			glm::vec3 APassthroughTransSpace = passthrough - TranslationSpaceVertexPositionA;

			glm::vec3 BATransSpace = TranslationSpaceVertexPositionA - TranslationSpaceVertexPositionB;
			glm::vec3 BCTransSpace = TranslationSpaceVertexPositionC - TranslationSpaceVertexPositionB;
			glm::vec3 BPassthroughTransSpace = passthrough - TranslationSpaceVertexPositionB;

			glm::vec3 CATransSpace = TranslationSpaceVertexPositionA - TranslationSpaceVertexPositionC;
			glm::vec3 CBTransSpace = TranslationSpaceVertexPositionB - TranslationSpaceVertexPositionC;
			glm::vec3 CPassthroughTransSpace = passthrough - TranslationSpaceVertexPositionC;

			bool withinASweep =
				abs(glm::angle(ACTransSpace, APassthroughTransSpace)) < abs(glm::angle(ACTransSpace, ABTransSpace)) &&
				abs(glm::angle(ABTransSpace, APassthroughTransSpace)) < abs(glm::angle(ACTransSpace, ABTransSpace));
			bool withinBSweep =
				abs(glm::angle(BATransSpace, BPassthroughTransSpace)) < abs(glm::angle(BATransSpace, BCTransSpace)) &&
				abs(glm::angle(BCTransSpace, BPassthroughTransSpace)) < abs(glm::angle(BCTransSpace, BATransSpace));
			bool withinCSweep =
				abs(glm::angle(CATransSpace, CPassthroughTransSpace)) < abs(glm::angle(CATransSpace, CBTransSpace)) &&
				abs(glm::angle(CBTransSpace, CPassthroughTransSpace)) < abs(glm::angle(CBTransSpace, CATransSpace));

			bool withinSweep = withinASweep && withinBSweep && withinCSweep;

			if (withinSweep)
			{
				std::cout << "Triangle collision" << std::endl;
				float AWeight = 1 / glm::length(APassthroughTransSpace);
				float BWeight = 1 / glm::length(BPassthroughTransSpace);
				float CWeight = 1 / glm::length(CPassthroughTransSpace);

				glm::vec3 weightedTranslationComponentSum =
					AWeight * TranslationSpaceComponentA +
					BWeight * TranslationSpaceComponentB +
					CWeight * TranslationSpaceComponentC;
				glm::vec3 destinationTranslationCompoonent = weightedTranslationComponentSum / (AWeight + BWeight + CWeight) + normalize(weightedTranslationComponentSum) * 0.01f;
				return destinationTranslationCompoonent + passthrough;
			}
		}

	}
	return destination;
}


	float numberColliding = 0.0;
	float numberInside = 0.0;
	float numAdjustments = 0.0;
	float numActiveVertices = 0.0;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> positions;
	glm::vec3 positionSum(0.0, 0.0, 0.0);
	glm::vec3 normalSum(0.0, 0.0, 0.0);
	glm::vec3 adjustedDestination = destination;

	// iterate through vertices of all collision Objects
	for (int vertexIndex = 0; vertexIndex < vertexPositions.size(); ++vertexIndex)
	{
		float threshold = 10.0;
		float lowestLength = threshold;
		glm::vec3 vertexPosition = vertexPositions[vertexIndex];
		glm::vec3 destinationVertexVec = vertexPosition - destination;
		glm::vec3 startVertexVec = vertexPosition - start;
		float vertexDestinationLength = glm::length(destinationVertexVec);
		float vertexStartLength = glm::length(startVertexVec);

		// Find Closest vertex and trigger collision
		if (vertexDestinationLength < threshold || vertexStartLength < threshold) {
			glm::vec3& normal = vertexNormals[vertexIndex];

			//if we are inside the object relative to the vertex move to the closest outside point along the movement path (second is excluding vertices already "passed")
			if (
				glm::dot(destinationVertexVec, vertexNormals[vertexIndex]) > 0 && 
				glm::dot(startVertexVec, vertexNormals[vertexIndex]) < 0 &&
				vertexStartLength < lowestLength)
			{

				glm::vec3 vertexNormalOrientedComponent = glm::proj(vertexPosition, normal);
				glm::vec3 positionNormalOrientedComponent = glm::proj(destination, normal);
				adjustedDestination = destination - positionNormalOrientedComponent + vertexNormalOrientedComponent;
			}
		}
	}
	return adjustedDestination;
}
*/