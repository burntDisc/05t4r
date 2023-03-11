#include "Char.h"

//TODO: Batch Draw Chars
Char::Char(char character, glm::vec2 origin, float size)
{
	//build character
    switch (character) 
	{
		case '0':
			Init_0();
			break;
		case '4':
			Init_4();
			break;
		case '5':
			Init_5();
			break;
		case 'a':
			Init_a();
			break;
		case 'b':
			// code block
			break;
		case 'r':
			Init_r();
			break;
		case 't':
			Init_t();
			break;
		default:
			// code block	
			break;
    }

	// adjust to screen position
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = origin + vertices[i].position * size;
	}

	SetVertices();
}


void Char::Init_0()
{
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

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_4()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);

	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_5()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(4);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.75f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));

}

void Char::Init_a()
{

}

void Char::Init_b()
{
}

void Char::Init_c()
{
}

void Char::Init_d()
{
}

void Char::Init_r()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_t()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(6);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, -0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.50f, -0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
}