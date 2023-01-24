#include "GeneratedGround.h"

GeneratedGround::GeneratedGround(
	Camera* camera,
	const char* modelFile,
	glm::vec3 initTranslation,
	glm::vec3 initScale,
	glm::quat initRotation) :
	camera(camera),
	panelLength(0.0f),
	panelWidth(0.0f),
	flipOffsetX(1),
	flipOffsetZ(1),
	GameObject(
		modelFile,
		initTranslation,
		initScale,
		initRotation)
{
	IntializePanelDimensions();
	UpdateWorldVertices();
}

void GeneratedGround::Update()
{
	// TODO maybe this should be built arouns square or this should be max (not width)
	visiblePanelsToEdge = (int)(camera->position.y / panelWidth  + 2)*4 -1;

	float xDiff = camera->position.x - translation.x;
	float zDiff = camera->position.z - translation.z;

	if (abs(xDiff) > panelWidth / 2)
	{
		translation.x += std::copysign(panelWidth, xDiff);
		flipOffsetX = (int)!flipOffsetX;
		validWorldPositions = false;
	}

	if (abs(zDiff) > panelLength / 2)
	{
		translation.z += std::copysign(panelLength, zDiff);
		flipOffsetZ = (int)!flipOffsetZ;
		validWorldPositions = false;
	}
	if (!validWorldPositions)
	{
		UpdateWorldVertices();
	}
}

glm::vec3 GeneratedGround::GetPanelScale(int panelIndex, int numToEdge)
{
	int sidePanels = (numToEdge * 2 + 1);
	float scaleOffsetX =
		((panelIndex / sidePanels + flipOffsetX) % 2) * scale.x * 2;
	float scaleOffsetZ =
		((panelIndex % sidePanels + flipOffsetZ) % 2) * 2 * scale.z;

	return scale - glm::vec3(scaleOffsetX, 0.0f, scaleOffsetZ);

}

glm::vec3 GeneratedGround::GetPanelTranslation(int panelIndex, int numToEdge)
{
	int sidePanels = (numToEdge * 2 + 1);
	float translationOffsetX =
		(panelIndex / sidePanels) * panelWidth - panelWidth * numToEdge;
	float translationOffsetZ =
		(panelIndex % sidePanels) * panelLength - panelLength * numToEdge;

	return translation - glm::vec3(translationOffsetX, 0.0f, translationOffsetZ);

}
void GeneratedGround::Draw(Shader& shader)
{
	int sidePanels = (visiblePanelsToEdge * 2 + 1);
	glDepthFunc(GL_LESS);

	for (int panelIndex = 0; panelIndex < sidePanels * sidePanels; ++panelIndex)
	{
		model.Draw(
			shader,
			GetPanelTranslation(panelIndex, visiblePanelsToEdge),
			rotation,
			GetPanelScale(panelIndex, visiblePanelsToEdge));
	}
}

void GeneratedGround::IntializePanelDimensions()
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
	float xMax = meshes[0].vertices[0].position.x;
	float xMin = meshes[0].vertices[0].position.x;
	float zMax = meshes[0].vertices[0].position.z;
	float zMin = meshes[0].vertices[0].position.z;

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

			xMax = vertexWorldPosition.x > xMax ? vertexWorldPosition.x : xMax;
			xMin = vertexWorldPosition.x < xMin ? vertexWorldPosition.x : xMin;
			zMax = vertexWorldPosition.z > zMax ? vertexWorldPosition.z : zMax;
			zMin = vertexWorldPosition.z < zMin ? vertexWorldPosition.z : zMin;
		}
	}
	panelWidth = xMax - xMin - buffer;
	panelLength = zMax - zMin - buffer;
}

void GeneratedGround::UpdateWorldVertices()
{
	triangleWorldPositions.clear();
	vertexWorldPositions.clear();
	triangleWorldNormals.clear();
	vertexWorldNormals.clear();

	std::vector<Mesh>& meshes = model.meshes;
	std::vector<glm::mat4>& transforms = model.meshTransforms;

	int sidePanels = (solidPanelsToEdge * 2 + 1);

	for (int panelIndex = 0; panelIndex < sidePanels * sidePanels; ++panelIndex)
	{
		glm::vec3 panelTranslation = GetPanelTranslation(panelIndex, solidPanelsToEdge);
		glm::vec3 panelScale = GetPanelScale(panelIndex, solidPanelsToEdge);

		// converting transformations to mat4 for combination
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		translationMatrix = glm::translate(translationMatrix, panelTranslation);
		rotationMatrix = glm::mat4_cast(rotation);
		scaleMatrix = glm::scale(scaleMatrix, panelScale);

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
				glm::vec3 vertexWorldPosition(translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
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
				triangleWorldPositions.push_back(worldPosition);
				triangleWorldNormals.push_back(worldNormal);
			}
		}
	}
	validWorldPositions = true;
}

