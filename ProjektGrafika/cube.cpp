#include "cube.h"

bool Cube::init()
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile("models\\cube.obj", aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	model = new ModelContext(scene->mMeshes[0]);

	return true;
}

void Cube::draw()
{
	glBindVertexArray(model->vertexArray);
	glDrawElements(GL_TRIANGLES, model->size, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}