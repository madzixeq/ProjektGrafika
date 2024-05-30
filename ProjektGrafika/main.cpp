#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include "ShaderProgram.h"
#include "ufo.h"
#include "asteroid.h"
#include "SceneHelper.h"
#include "asteroidProps.h"
#include <vector>
#include "lodepng.h"

const float PI = 3.141592653589793f;
float aspectRatio = 1;

float cameraSpeed = 0;
float cameraPosX = 0;

float asteroidSpeed = -5.0f;

bool isGameLost = false;

ShaderProgram* sp;
ShaderProgram* sp_simple;
UFO* ufo_model = new UFO();
Asteroid* asteroid_model = new Asteroid();
Cube* background = new Cube();
std::vector<AsteroidProps> asteroidsList;

GLuint texUFO;
GLuint texAsteroid;
GLuint texBackground;

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pami�ci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT && !isGameLost)
			cameraSpeed = -3.0f;
		if (key == GLFW_KEY_RIGHT && !isGameLost)
			cameraSpeed = 3.0f;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT)
			cameraSpeed = 0.0f;
		if (key == GLFW_KEY_RIGHT)
			cameraSpeed = 0.0f;
	}
}

void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	sp = new ShaderProgram("v_shader.glsl", NULL, "f_shader.glsl");
	sp_simple = new ShaderProgram("v_simple_shader.glsl", NULL, "f_simple_shader.glsl");

	ufo_model->init();
	asteroid_model->init();
	background->init();

	for (int i = 0; i < 10; i++) 
	{
		AsteroidProps tmp = {};
		tmp.posX = (float)(rand() % 100 - 50) / 10;
		tmp.posZ = 20.0f - i * 2;
		tmp.scale = (float)(rand() % 201 + 100) / 100;
		tmp.xAxis = (float)(rand() % 11) / 10;
		tmp.yAxis = (float)(rand() % 11) / 10;
		tmp.zAxis = (float)(rand() % 11) / 10;
		tmp.rotateSpeed = PI / ((float)(rand() % 31 + 10) / 10);
		tmp.rotateAngle = 0;
		asteroidsList.push_back(tmp);
	}

	texUFO = readTexture("textures\\spaceship_texture.png");
	texAsteroid = readTexture("textures\\asteroid_texture.png");
	texBackground = readTexture("textures\\background.png");
}

void drawScene(GLFWwindow* window) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	SceneHelper::drawUFO(ufo_model, sp, aspectRatio, cameraPosX, texUFO);

	SceneHelper::drawBackground(background, sp_simple, cameraPosX, texBackground);

	for(int i = 0; i < asteroidsList.size(); i++)
		SceneHelper::drawAsteroid(asteroid_model, sp, aspectRatio, cameraPosX, asteroidsList[i], texAsteroid);


	glfwSwapBuffers(window);
}

void handleAsteroids() 
{
	for (int i = 0; i < asteroidsList.size(); i++)
	{
		asteroidsList[i].posZ += asteroidSpeed * glfwGetTime();
		asteroidsList[i].rotateAngle += asteroidsList[i].rotateSpeed * glfwGetTime();
		if (asteroidsList[i].posZ <= -4) {
			asteroidsList[i].posZ = 20.0f;
			asteroidsList[i].posX = (float)(rand() % 100 - 50) / 10;
			asteroidsList[i].scale = (float)(rand() % 101 + 100) / 100;
			asteroidsList[i].xAxis = (float)(rand() % 11) / 10;
			asteroidsList[i].yAxis = (float)(rand() % 11) / 10;
			asteroidsList[i].zAxis = (float)(rand() % 11) / 10;
			asteroidsList[i].rotateSpeed = PI / ((float)(rand() % 31 + 10) / 10);
			asteroidsList[i].rotateAngle = 0;
		}
	}
}

void checkCollisions()
{
	for (int i = 0; i < asteroidsList.size(); i++)
	{
		double d = sqrt((asteroidsList[i].posX + cameraPosX) * (asteroidsList[i].posX + cameraPosX) + (asteroidsList[i].posZ + 1.5) * (asteroidsList[i].posZ + 1.5));
		if (d <= (0.155 * asteroidsList[i].scale + 0.42)) 
		{
			isGameLost = true;
			cameraSpeed = 0;
			asteroidSpeed = 0;
			for (int i = 0; i < asteroidsList.size(); i++)
			{
				asteroidsList[i].rotateSpeed = 0;
			}
		}
	}
}

int main(int argc, char* argv[]) 
{
	srand(time(0));
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) 
	{
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}

	float width = 1500, height = 1000;
	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	aspectRatio = (float)width / (float)height;
	if (!window)
	{
		fprintf(stderr, "Nie mo�na utworzy� okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	glfwSetTime(0);
	while (!glfwWindowShouldClose(window))
	{
		drawScene(window);

		handleAsteroids();
		checkCollisions();

		cameraPosX += cameraSpeed * glfwGetTime();
		if (cameraPosX >= 5.0f)
			cameraPosX = 5.0f;
		if (cameraPosX <= -5.0f)
			cameraPosX = -5.0f;

		glfwSetTime(0);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}