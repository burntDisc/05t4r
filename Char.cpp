#include "Char.h"

//TODO: Batch Draw Chars

//TODO: fix weird scaling from center (height of 2)
Char::Char(Shader shader, char character, glm::vec2 origin, float size) : 
	Overlay(shader), origin(origin), size(size)
{
	//build character
	InitVertices(character);
	// adjust to screen position
	SetCharPosition();
	SetVertices();
}

void Char::SetCharPosition() 
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = origin + vertices[i].position * size;
	}
}

void Char::InitVertices(char character)
{
	switch (character)
	{
	case '.':
		Init_dot();
		break;
	case '0':
		Init_0();
		break;
	case '1':
		Init_1();
		break;
	case '2':
		Init_2();
		break;
	case '3':
		Init_3();
		break;
	case '4':
		Init_4();
		break;
	case '5':
		Init_5();
		break;
	case '6':
		Init_6();
		break;
	case '7':
		Init_7();
		break;
	case '8':
		Init_8();
		break;
	case '9':
		Init_9();
		break;
	case 'a':
		Init_a();
		break;
	case 'e':
		Init_e();
		break;
	case 'p':
		Init_p();
		break;
	case 'r':
		Init_r();
		break;
	case 's':
		Init_s();
		break;
	case 't':
		Init_t();
		break;
	default:
		// code block	
		break;
	}
}
void Char::Change(char newCharacter)
{
	indices.clear();
	vertices.clear();
	InitVertices(newCharacter);
	SetCharPosition();
	SetVertices();
}

void Char::Delete()
{
	delete this;
}

void Char::Init_dot()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
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

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_1()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(0.2f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.6f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.6f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.4f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_2()
{
	//top
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// down angled part
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(2);
	indices.push_back(4);
	indices.push_back(5);

	// bottom
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.8f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_3()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);

	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(2);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
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

	vertices.push_back(Vertex2D(glm::vec2(0.75f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.66), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
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

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

}

void Char::Init_6()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.2f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_7()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.4f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_8()
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

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(5);
	indices.push_back(8);
	indices.push_back(9);

	indices.push_back(10);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(10);
	indices.push_back(8);
	indices.push_back(9);

	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_9()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.9f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.8f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.9f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_a()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.45f, 0.42f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.95f, 0.05f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.55f, 0.42f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.30f, 0.42f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.70f, 0.42f), glm::vec3(0.0f, 0.0f, 0.0f)));

}

void Char::Init_e()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);

	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(2);

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.75f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.25f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_p()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.9f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.8f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.1f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_r()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);


	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.72f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.55f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.63f), glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Char::Init_s()
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

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
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

	vertices.push_back(Vertex2D(glm::vec2(0.0f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.66f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.33f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(1.0f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.25f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.75f, 0.50f), glm::vec3(0.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex2D(glm::vec2(0.50f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
}