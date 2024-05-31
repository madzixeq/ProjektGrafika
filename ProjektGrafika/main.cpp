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
#include <ft2build.h>
#include "fontHelper.h"

const float PI = 3.141592653589793f;
float aspectRatio = 1;

float cameraSpeed = 0;
float cameraPosX = 0;

float screenX = 1500;
float screenY = 1000;

float asteroidSpeed = 0.0f;

int points;

clock_t speedChangeStart;
clock_t speedChangeEnd;
clock_t addPointsStart;
clock_t addPointsEnd;

bool isGameLost = true;
bool isFirstGame = true;

ShaderProgram* sp;
ShaderProgram* sp_simple;
ShaderProgram* sp_text;

UFO* ufo_model = new UFO();
Asteroid* asteroid_model = new Asteroid();
Cube* background = new Cube();

std::vector<AsteroidProps> asteroidsList;

GLuint texUFO;
GLuint texAsteroid;
GLuint texBackground;

FontHelper* fh = new FontHelper();

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

std::string scoreToText(int score)
{
	return "SCORE: " + std::to_string(score);
}

void restartGame()
{
	asteroidsList = {};
	for (int i = 0; i < 20; i++)
	{
		AsteroidProps tmp = {};
		tmp.posX = (float)(rand() % 100 - 50) / 10;
		tmp.posZ = 50.0f - i * 2;
		tmp.scale = (float)(rand() % 201 + 100) / 100;
		tmp.xAxis = (float)(rand() % 11) / 10;
		tmp.yAxis = (float)(rand() % 11) / 10;
		tmp.zAxis = (float)(rand() % 11) / 10;
		tmp.rotateSpeed = PI / ((float)(rand() % 31 + 10) / 10);
		tmp.rotateAngle = 0;
		asteroidsList.push_back(tmp);
	}
	asteroidSpeed = -5.0f;

	cameraPosX = 0.0;
	cameraSpeed = 0;
	speedChangeStart = clock();
	addPointsStart = clock();
	isGameLost = false;
	points = 0;
	isFirstGame = false;
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	screenX = width;
	screenY = height;
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT && !isGameLost)
			cameraSpeed = -4.5f;
		if (key == GLFW_KEY_RIGHT && !isGameLost)
			cameraSpeed = 4.5f;
		if (key == GLFW_KEY_SPACE && isGameLost)
			restartGame();
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sp = new ShaderProgram("v_shader.glsl", NULL, "f_shader.glsl");
	sp_simple = new ShaderProgram("v_simple_shader.glsl", NULL, "f_simple_shader.glsl");
	sp_text = new ShaderProgram("v_text_shader.glsl", NULL, "f_text_shader.glsl");

	ufo_model->init();
	asteroid_model->init();
	background->init();
	fh->init();

	texUFO = readTexture("textures\\spaceship_texture.png");
	texAsteroid = readTexture("textures\\asteroid_texture.png");
	texBackground = readTexture("textures\\background.png");
}

void drawScene(GLFWwindow* window) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isGameLost && isFirstGame) 
	{
		fh->render(sp_text, "Press SPACE to start", screenX / 2 - 250, screenY / 2 + 50, screenX, screenY, 1.0);
	}
	else if (isGameLost)
	{
		fh->render(sp_text, scoreToText(points), screenX / 2 - scoreToText(points).size() * 12, screenY / 2 + 90, screenX, screenY, 1.0);
		fh->render(sp_text, "Press SPACE to restart", screenX / 2 - 220, screenY / 2, screenX, screenY, 0.75);
	}
	else 
	{
		fh->render(sp_text, scoreToText(points), 10, screenY - 50, screenX, screenY, 1.0);
	}

	SceneHelper::drawUFO(ufo_model, sp, aspectRatio, cameraPosX, cameraSpeed, texUFO);

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
			asteroidsList[i].posZ = 40.0f;
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

void handleTime() 
{
	if (!isGameLost) 
	{
		speedChangeEnd = clock();
		if ((double)(speedChangeEnd - speedChangeStart) >= 800) 
		{
			asteroidSpeed -= 0.1;
			speedChangeStart = clock();
		}

		addPointsEnd = clock();
		if ((double)(addPointsEnd - addPointsStart) >= 100)
		{
			points -= asteroidSpeed;
			addPointsStart = clock();
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
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(screenX, screenY, "OpenGL", NULL, NULL);
	aspectRatio = (float)screenX / (float)screenY;
	if (!window)
	{
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	glfwSetTime(0);
	speedChangeStart = clock();
	addPointsStart = clock();
	while (!glfwWindowShouldClose(window))
	{
		drawScene(window);

		handleAsteroids();
		checkCollisions();
		handleTime();

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