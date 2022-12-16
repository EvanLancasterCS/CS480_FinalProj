#include "skybox.h"

skybox::skybox()
{
	const std::vector<const char*> faces =
	{
		"assets\\Skybox\\Right.png",
		"assets\\Skybox\\Left.png",
		"assets\\Skybox\\Top.png",
		"assets\\Skybox\\Bottom.png",
		"assets\\Skybox\\Front.png",
		"assets\\Skybox\\Back.png"
	};
	texture = new Texture(faces);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

skybox::~skybox() = default;

void skybox::Render(Shader* shader, Camera* camera)
{
	glDepthFunc(GL_LEQUAL);
	shader->Enable();
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetView()));
	glm::mat4 projection = camera->GetProjection();
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
