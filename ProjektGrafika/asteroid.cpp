#include "asteroid.h"

bool Asteroid::init()
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile("models\\asteroid.obj", aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	model = new ModelContext(scene->mMeshes[0]);

	return true;
}

void Asteroid::draw()
{
	glBindVertexArray(model->vertexArray);
	glDrawElements(GL_TRIANGLES, model->size, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}