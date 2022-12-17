#include "mesh.h"

Mesh::Mesh()
{
	// Vertex Set Up
	// No mesh

	// Model Set Up
	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = glm::translate(glm::mat4(1.0f), pivotLocation);
	position = glm::vec3(0.f, 0.f, 0.f);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}

}

Mesh::Mesh(glm::vec3 pivot, const char* fname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);
	position = glm::vec3(0.f, 0.f, 0.f);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	hasTexture = false;
	hasNormal = false;
}

Mesh::Mesh(glm::vec3 pivot, const char* fname, const char* tname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);
	position = glm::vec3(0.f, 0.f, 0.f);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	// load texture from file
	m_texture = new Texture(tname, true);
	if (m_texture)
		hasTexture = true;
	else
		hasTexture = false;
	hasNormal = false;
}


Mesh::~Mesh()
{
	Vertices.clear();
	Indices.clear();
}

void Mesh::Update(glm::mat4 matModel, double dt)
{

	// calculate x,y,z vector for forward based on pitch/yaw
	float xzLen = glm::cos(glm::radians(pitch));
	float x = xzLen * glm::sin(glm::radians(yaw));
	float y = -glm::sin(glm::radians(pitch));
	float z = xzLen * glm::cos(glm::radians(yaw));

	forward = glm::vec3(x, y, z);
	right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	up = glm::normalize(glm::cross(right, forward));

	position = position + velocity;

	matModel *= glm::translate(glm::mat4(1.f), position);

	positionMatrix = matModel;

	// rotate model based on yaw/pitch/roll
	matModel *= glm::rotate(glm::mat4(1.f), glm::radians(yaw), glm::vec3(0, 1, 0));
	matModel *= glm::rotate(glm::mat4(1.f), glm::radians(pitch), glm::vec3(1, 0, 0));
	matModel *= glm::rotate(glm::mat4(1.f), glm::radians(roll), glm::vec3(0, 0, 1));


	matModel *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	model = matModel;
}

void Mesh::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw -= xoffset;
	pitch -= yoffset;

	// Bounds checking
	if (pitch > 76.0f)
		pitch = 76.0f;
	if (pitch < -76.0f)
		pitch = -76.0f;
}

void Mesh::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float acceleration = 0.0005f;
	switch (direction)
	{
	case FORWARD:
		velocity += forward * acceleration;
		break;
	case BACKWARD:
		velocity -= forward * acceleration;
		break;
	case LEFT:
		velocity -= right * acceleration;
		break;
	case RIGHT:
		velocity += right * acceleration;
		break;
	case ROLL_CW:
		//roll += 0.5f;
		break;
	case ROLL_CCW:
		//roll -= 0.5f;
		break;
	case BRAKE:
		velocity -= velocity / 10.f;
		break;
	default:
		break;
	}
}

glm::mat4 Mesh::GetModel()
{
	return model;
}

void Mesh::Render(Shader* m_shader, Camera* m_camera)
{
	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
	if (hasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getTextureID());
		GLuint sampler = m_shader->GetUniformLocation("textureSP");
		if (hasNormal)
			sampler = m_shader->GetUniformLocation("normalSP");
		if (sampler == INVALID_UNIFORM_LOCATION)
			printf("Sampler Not found not found\n");
		glUniform1i(sampler, 0);
	}

	glBindVertexArray(vao);
	// Enable vertex attribute arrays for each vertex attrib
	glEnableVertexAttribArray(m_positionAttrib);
	glEnableVertexAttribArray(m_colorAttrib);
	glEnableVertexAttribArray(m_tcAttrib);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
	glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(m_colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glVertexAttribPointer(m_tcAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoord)));

	// If has texture, set up texture unit(s): update here for texture rendering
	if (m_texture != nullptr)
		glUniform1i(m_hasTexture, true);
	else
		glUniform1i(m_hasTexture, false);

	if (m_normalTexture != nullptr)
		glUniform1i(m_hasNormal, true);
	else
		glUniform1i(m_hasNormal, false);

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

	// Disable vertex arrays
	glDisableVertexAttribArray(m_positionAttrib);
	glDisableVertexAttribArray(m_colorAttrib);
	glDisableVertexAttribArray(m_tcAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Mesh::InitBuffers() {

	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

bool Mesh::loadModelFromFile(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene) {
		printf("couldn't open the .obj file. \n");
		return false;
	}

	const int ivertTotalSize = 2 * sizeof(aiVector3D);

	int iTotalVerts = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++) {
				// update here for each mesh's vertices to assign position, normal, and texture coordinates
				Vertices.emplace_back(glm::vec3(mesh->mVertices[face.mIndices[k]].x, mesh->mVertices[face.mIndices[k]].y, mesh->mVertices[face.mIndices[k]].z),
					mesh->HasNormals() ? glm::vec3(mesh->mNormals[face.mIndices[k]].x, mesh->mNormals[face.mIndices[k]].y, mesh->mNormals[face.mIndices[k]].z) : glm::vec3(1.f, 1.f, 1.f),
					mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][face.mIndices[k]].x, mesh->mTextureCoords[0][face.mIndices[k]].y) : glm::vec2(1.f, 0.f));
			}
		}
		iTotalVerts += mesh->mNumVertices;
	}
	for (int i = 0; i < Vertices.size(); i++) {
		Indices.push_back(i);
	}
}

bool Mesh::ShaderInfo(Shader* shader)
{
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = shader->GetAttribLocation("v_normal");
	if (m_colorAttrib == -1)
	{
		printf("v_normal attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	m_hasNormal = shader->GetUniformLocation("hasNormal");
	if (m_hasNormal == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}