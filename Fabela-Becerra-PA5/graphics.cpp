#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader("Shaders\\core.vs","Shaders\\core.frag");
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}
	// Set up the sky shaders
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}
	m_skybox = new skybox();

	m_skyshader = new Shader("Shaders\\sky.vs", "Shaders\\sky.frag");


	if (!m_skyshader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}
	// Set up the sky shaders
	if (!m_skyshader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_skyshader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_skyshader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_skyshader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Starship
	m_mesh = new Mesh(glm::vec3(0.0f, 0.0f, 0.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	m_mesh->ShaderInfo(m_shader);
	m_mesh->SetRotSpeed(1);
	m_mesh->SetSpeed({ 1,1,1 });
	m_mesh->SetScale({ .01,.01,.01 });
	m_mesh->SetDist({ 1,1,1 });
	m_mesh->SetRotVector({ 1,1,1 });

	// Asteroid Belt
	m_asteroid = new Asteroid(glm::vec3(0.0f, 0.0f, 0.0f), "assets\\rock.obj", "assets\\rock.png");

	m_asteroid->ShaderInfo(m_shader);
	m_asteroid->SetRotSpeed(1);
	m_asteroid->SetSpeed({ 1,1,1 });
	m_asteroid->SetScale({ .01,.01,.01 });
	m_asteroid->SetDist({ 1,1,1 });
	m_asteroid->SetRotVector({ 1,1,1 });


	// The Sun
	m_sphere = new Sphere(64, "assets\\2k_sun.jpg","assets\\Sun-n.png");

	m_sphere->ShaderInfo(m_shader);
	m_sphere->SetRotSpeed(1);
	m_sphere->SetSpeed({ 1,1,1 });
	m_sphere->SetScale({ .2,.2,.2 });
	m_sphere->SetDist({ 0,0,0 });
	m_sphere->SetRotVector({ 1,1,1 });
	//second sun
	m_sphere2 = new Sphere(64, "assets\\2k_sun.jpg", "assets\\Sun-n.png");

	m_sphere2->ShaderInfo(m_shader);
	m_sphere2->SetRotSpeed(1);
	m_sphere2->SetSpeed({ 1,1,1 });
	m_sphere2->SetScale({ .2,.2,.2 });
	m_sphere2->SetDist({ 2,2,2 });
	m_sphere2->SetRotVector({ 1,1,1 });

	/*m_sphere3 = new Sphere(64, "assets\\2k_sun.jpg", "assets\\Sun-n.png");

	m_sphere3->ShaderInfo(m_shader);
	m_sphere3->SetRotSpeed(1);
	m_sphere3->SetSpeed({ 1,1,1 });
	m_sphere3->SetScale({ .2,.2,.2 });
	m_sphere3->SetDist({ 2,2,2 });
	m_sphere3->SetRotVector({ 1,1,1 });*/



	//The Earth
	m_sphere3 = new Sphere(48, "assets\\2k_earth_daymap.jpg","assets\\2k_earth_daymap-n.png");
	
	m_sphere3->ShaderInfo(m_shader);
	m_sphere3->SetRotSpeed(1);
	m_sphere3->SetSpeed({ 1,1,1 });
	m_sphere3->SetScale({ .2,.2,.2 });
	m_sphere3->SetDist({ 2,2,2 });
	m_sphere3->SetRotVector({ 1,1,1 });

	//// The moon
	//m_sphere3 = new Sphere(48, "assets\\2k_moon.jpg");
	//m_sphere3->ShaderInfo(m_shader);

	/*m_sphere->SetRotSpeed(1);
	m_sphere->SetSpeed({1,1,1});
	m_sphere->SetScale({1,1,1});
	m_sphere->SetDist({ 1,1,1 });
	m_sphere->SetRotVector({ 1,1,1 });*/

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {
	//getposition locks moon to planet
	//glm vec 3 change location of object
	m_mesh->Update(glm::translate(glm::mat4(1.f),glm::vec3(0,0,0)), dt);
	m_sphere->Update(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), dt);
	m_sphere2->Update(m_sphere->GetPositionMatrix(), dt);
	m_sphere3->Update(glm::translate(glm::mat4(1.f), glm::vec3(3, 0, 3)), dt);
	m_asteroid->Update(glm::translate(glm::mat4(1.f), glm::vec3(-3, 0, -3)), dt);
}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	//glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();


	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	m_skybox->Render(m_skyshader, m_camera);


	m_mesh->Render(m_shader, m_camera);
	m_sphere->Render(m_shader);
	m_sphere2->Render(m_shader);
	m_sphere3->Render(m_shader);
	//instancing this to make belt
	m_asteroid->Render(m_shader,m_camera);
	
	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_normal");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

