#include "Title.h"

Title::Title() :
	Overlay::Overlay()
{
	float unit = 0.10f;

	// 0
	float xUnitOffset = -10.0f;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(4);

	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, 3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(3.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, -3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	// 5
	xUnitOffset += 4.0f;
	int indexOffset = vertices.size();
	indices.push_back(0 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(0 + indexOffset);
	indices.push_back(2 + indexOffset);
	indices.push_back(3 + indexOffset);

	indices.push_back(2 + indexOffset);
	indices.push_back(3 + indexOffset);
	indices.push_back(4 + indexOffset);

	indices.push_back(5 + indexOffset);
	indices.push_back(3 + indexOffset);
	indices.push_back(4 + indexOffset);

	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(3.0f + xUnitOffset, -3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, -3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	// t
	xUnitOffset += 4.0f;
	indexOffset = vertices.size();
	indices.push_back(0 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(3 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(4 + indexOffset);
	indices.push_back(5 + indexOffset);
	indices.push_back(6 + indexOffset);

	indices.push_back(7 + indexOffset);
	indices.push_back(5 + indexOffset);
	indices.push_back(6 + indexOffset);

	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, 1.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, -1.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, 3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(3.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, -3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	// 4
	xUnitOffset += 4.0f;
	indexOffset = vertices.size();
	indices.push_back(0 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(3 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(0 + indexOffset);
	indices.push_back(4 + indexOffset);
	indices.push_back(5 + indexOffset);

	vertices.push_back(Vertex2D(glm::vec2(3.0f + xUnitOffset, 3.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, 1.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(2.0f + xUnitOffset, 0.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(3.0f + xUnitOffset, -3.0f) * unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	// r
	xUnitOffset += 4.0f;
	indexOffset = vertices.size();
	indices.push_back(0 + indexOffset);
	indices.push_back(1 + indexOffset);
	indices.push_back(2 + indexOffset);

	indices.push_back(3 + indexOffset);
	indices.push_back(4 + indexOffset);
	indices.push_back(5 + indexOffset);

	vertices.push_back(Vertex2D(glm::vec2(0.0f + xUnitOffset, 0.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, 3.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, -3.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, 2.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f + xUnitOffset, 0.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(4.0f + xUnitOffset, 1.0f)* unit, glm::vec3(0.0f, 0.0f, 0.0f)));

	SetVertices();
}