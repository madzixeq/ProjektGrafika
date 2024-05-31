#pragma once
#include "ufo.h"
#include "asteroid.h"
#include "cube.h"
#include "ShaderProgram.h"
#include "asteroidProps.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneHelper
{
public:
	static void drawUFO(UFO* model, ShaderProgram* sp, float aspectRatio, float cameraPosX, float cameraSpeed, GLuint tex);
	static void drawAsteroid(Asteroid* model, ShaderProgram* sp, float aspectRatio, float cameraPosX, AsteroidProps props, GLuint tex);
	static void drawBackground(Cube* model, ShaderProgram* sp, float cameraPosX, GLuint tex);
};

