#pragma once
#include <glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelContext
{
public:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint vertexIndexBuffer;
	int size = 0;

	ModelContext(aiMesh* mesh);
};

