#include "SceneHelper.h"
#include <glfw3.h>

void SceneHelper::drawUFO(UFO* model, ShaderProgram* sp, float aspectRatio, float cameraPosX, float cameraSpeed, GLuint tex)
{
	float PI = 3.141592653589793f;
	float rotateAngle = 0;

	if (cameraSpeed > 0)
		rotateAngle = PI / 12;
	else if (cameraSpeed < 0)
		rotateAngle = -PI / 12;

	sp->use();

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(-cameraPosX, 0.0f, -1.5f));
	M = glm::rotate(M, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 1.0f, 50.0f);
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glm::mat4 V = glm::lookAt(
		glm::vec3(-cameraPosX, 2, -4),
		glm::vec3(-cameraPosX, 0, 0),
		glm::vec3(0.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniform4f(sp->u("lp"), -cameraPosX, 0.5, -0.5, 1.0);

	model->draw();

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void SceneHelper::drawAsteroid(Asteroid* model, ShaderProgram* sp, float aspectRatio, float cameraPosX, AsteroidProps props, GLuint tex)
{
	float PI = 3.141592653589793f;

	sp->use();

	glUniform1i(sp->u("textureMap0"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(props.posX, 0.0f, props.posZ));
	M = glm::rotate(M, props.rotateAngle, glm::vec3(props.xAxis, props.yAxis, props.zAxis));
	M = glm::scale(M, glm::vec3(0.002f * props.scale, 0.002f * props.scale, 0.002f * props.scale));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 1.0f, 50.0f);
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glm::mat4 V = glm::lookAt(
		glm::vec3(-cameraPosX, 2, -4),
		glm::vec3(-cameraPosX, 0, 0),
		glm::vec3(0.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniform4f(sp->u("lp"), 0.0, 0.0, 100.0, 1.0);

	model->draw();

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void SceneHelper::drawBackground(Cube* model, ShaderProgram* sp, float cameraPosX, GLuint tex)
{
	float PI = 3.141592653589793f;

	sp->use();

	glUniform1i(sp->u("tex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, 50.0f));
	M = glm::scale(M, glm::vec3(30.0f, 30.0f, 10.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 1.0f, 50.0f);
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glm::mat4 V = glm::lookAt(
		glm::vec3(cameraPosX/2, 0, -4),
		glm::vec3(cameraPosX/2, 0, 0),
		glm::vec3(0.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	model->draw();

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("texCoord"));
}

