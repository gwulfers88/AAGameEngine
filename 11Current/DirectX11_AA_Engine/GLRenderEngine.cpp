#include "GLRenderEngine.h"
#include "constants.h"
#include "GLCamera.h"

Transform transform;
GLCamera* camera = 0;

GLRenderEngine::GLRenderEngine() : GLGraphicsEngine()
{
	transform.SetPosition(vmath::vec3(0.0f, -25.0f, 0.0f));
	transform.SetRotation(vmath::vec3(-90.0f, 0.0f, 0.0f));

	camera = new GLCamera(vmath::vec3(0, 0, -100.0f), 50.0f, (float)config->windowWidth / (float)config->windowHeight, 0.1f, 1000.0f);
}

GLRenderEngine::~GLRenderEngine()
{
	delete camera;
}

void GLRenderEngine::Update(float dt)
{
	float aspect = (float)config->windowWidth / (float)config->windowHeight;
	camera->UpdateAspectRatio(aspect);
}

void GLRenderEngine::Render(float dt)
{
	glViewport(0, 0, config->windowWidth, config->windowHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	glBindVertexArray(vao);

	shader.Bind();

	glUniformMatrix4fv(shader.GetUniformLocation(UT_PROJECTION), 1, GL_FALSE, camera->ToMatrix());

	for (int i = 0; i < 1; i++)
	{
		float f = i + dt * 0.0035f;
		
		//transform.GetRotation()[0] = dt * .50f;
		transform.GetRotation()[1] = dt * .20f;

		/*transform.GetPosition()[0] = sinf(1.5f * f) * 5.0f;
		transform.GetPosition()[1] = cosf(1.2f * f) * 5.0f;
		transform.GetPosition()[2] = sinf(1.5f * f) * cosf(.5f * f) * 2.0f;*/

		glUniformMatrix4fv(shader.GetUniformLocation(UT_MODEL_VIEW), 1, GL_FALSE, transform.ToMatrix());

		//if (i % 2)
		{
			glDrawElements(GL_TRIANGLES, cube.indexCount, GL_UNSIGNED_INT, 0);
		}
		//else
		{
			//glDrawElements(GL_LINES, ArrayCount(indices), GL_UNSIGNED_INT, 0);
		}
	}

	shader.Unbind();

	glBindVertexArray(0);
}