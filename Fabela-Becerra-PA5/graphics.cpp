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

	m_light = new Light(m_camera->GetView());

	// Set up the shaders
	m_shader = new Shader("Shaders\\core.vs","Shaders\\core.frag");
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}
	/*
	// Set up the sky shaders
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}*/

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

	//// Asteroid Belt
	//m_asteroid = new Asteroid(glm::vec3(0.0f, 0.0f, 0.0f), "assets\\rock.obj", "assets\\rock.png");

	//m_asteroid->ShaderInfo(m_shader);
	//m_asteroid->SetRotSpeed(1);
	//m_asteroid->SetSpeed({ 1,1,1 });
	//m_asteroid->SetScale({ .01,.01,.01 });
	//m_asteroid->SetDist({ 1,0,1 });
	//m_asteroid->SetRotVector({ 1,0,1 });


	// The Sun
	m_sun = new Sphere(64, "assets\\2k_sun.jpg","assets\\Sun-n.png");

	m_sun->ShaderInfo(m_shader);
	m_sun->SetRotSpeed(1);
	m_sun->SetSpeed({ 1,1,1 });
	m_sun->SetScale({ .2,.2,.2 });
	m_sun->SetDist({ 0,0,0 });
	m_sun->SetRotVector({ 0,1,0 });
	m_sun->matAmbient[0] = 4;
	m_sun->matAmbient[1] = 4;
	m_sun->matAmbient[2] = 4;
	m_sun->matAmbient[3] = 4;

	//Mercury
	m_mercury = new Sphere(64, "assets\\Mercury.jpg", "assets\\Mercury-n.jpg");

	m_mercury->ShaderInfo(m_shader);
	m_mercury->SetRotSpeed(1);
	m_mercury->SetSpeed({ .5,.5,.5 });
	m_mercury->SetScale({ .2,.2,.2 });
	m_mercury->SetDist({ .75,.0,.75 });
	m_mercury->SetRotVector({ 1,0,1 });

	//venus
	m_venus = new Sphere(64, "assets\\Venus.jpg", "assets\\Venus-n.jpg");

	m_venus->ShaderInfo(m_shader);
	m_venus->SetRotSpeed(1);
	m_venus->SetSpeed({ .5,.5,.5 });
	m_venus->SetScale({ .2,.2,.2 });
	m_venus->SetDist({ 1.5,0,1.5 });
	m_venus->SetRotVector({ 1,0,1 });

	//The Earth
	m_earth = new Sphere(48, "assets\\2k_earth_daymap.jpg","assets\\2k_earth_daymap-n.jpg");
	
	m_earth->ShaderInfo(m_shader);
	m_earth->SetRotSpeed(1);
	m_earth->SetSpeed({ .5,.5,.5 });
	m_earth->SetScale({ .2,.2,.2 });
	m_earth->SetDist({ 2,0,2 });
	m_earth->SetRotVector({ 1,0,1 });

	//mars
	m_mars = new Sphere(48, "assets\\Mars.jpg", "assets\\Mars-n.jpg");

	m_mars->ShaderInfo(m_shader);
	m_mars->SetRotSpeed(1);
	m_mars->SetSpeed({ .5,.5,.5 });
	m_mars->SetScale({ .2,.2,.2 });
	m_mars->SetDist({ 4,0,4 });
	m_mars->SetRotVector({ 1,0,1 });

	//jupiter
	m_jupiter = new Sphere(48, "assets\\Jupiter.jpg", "assets\\Jupiter-n.jpg");

	m_jupiter->ShaderInfo(m_shader);
	m_jupiter->SetRotSpeed(1);
	m_jupiter->SetSpeed({ .5,.5,.5 });
	m_jupiter->SetScale({ .2,.2,.2 });
	m_jupiter->SetDist({ 10,0,10 });
	m_jupiter->SetRotVector({ 1,0,1 });

	//saturn
	m_saturn = new Sphere(48, "assets\\Saturn.jpg", "assets\\Saturn_ring.png");

	m_saturn->ShaderInfo(m_shader);
	m_saturn->SetRotSpeed(1);
	m_saturn->SetSpeed({ .5,.5,.5 });
	m_saturn->SetScale({ .2,.2,.2 });
	m_saturn->SetDist({ 15,0,15 });
	m_saturn->SetRotVector({ 1,0,1 });

	//neptune
	m_neptune = new Sphere(48, "assets\\Neptune.jpg", "assets\\Neptune-n.jpg");

	m_neptune->ShaderInfo(m_shader);
	m_neptune->SetRotSpeed(1);
	m_neptune->SetSpeed({ .5,.5,.5 });
	m_neptune->SetScale({ .2,.2,.2 });
	m_neptune->SetDist({ 20,0,20 });
	m_neptune->SetRotVector({ 1,0,1 });
	

	//The moon
	m_moon = new Sphere(48, "assets\\2k_moon.jpg", "assets\\2k_moon-n.jpg");
	m_moon->ShaderInfo(m_shader);

	m_moon->SetRotSpeed(1);
	m_moon->SetSpeed({.5,.5,.5});
	m_moon->SetScale({.05,.05,.05});
	m_moon->SetDist({ 1,1,1 });
	m_moon->SetRotVector({ 0,1,0});

	//The ceres
	m_ceres = new Sphere(48, "assets\\Ceres.jpg", "assets\\Ceres-n.jpg");
	m_ceres->ShaderInfo(m_shader);

	m_ceres->SetRotSpeed(1);
	m_ceres->SetSpeed({ -1,-1,1 });
	m_ceres->SetScale({ .2,.2,.2 });
	m_ceres->SetDist({ 8,8,8 });
	m_ceres->SetRotVector({ 1,1,1 });

	//The eris
	m_eris = new Sphere(48, "assets\\Eris.jpg", "assets\\Eris-n.jpg");
	m_eris->ShaderInfo(m_shader);

	m_eris->SetRotSpeed(1);
	m_eris->SetSpeed({ -1,-1,1 });
	m_eris->SetScale({ .2,.2,.2 });
	m_eris->SetDist({ 2,2,2 });
	m_eris->SetRotVector({ 1,1,1 });

	//The haumea
	m_haumea = new Sphere(48, "assets\\Haumea.jpg", "assets\\Haumea-n.jpg");
	m_haumea->ShaderInfo(m_shader);

	m_haumea->SetRotSpeed(1);
	m_haumea->SetSpeed({ -1,-1,1 });
	m_haumea->SetScale({ .2,.2,.2 });
	m_haumea->SetDist({ 4,4,4 });
	m_haumea->SetRotVector({ 1,1,1 });

	// Comet haileys
	m_asteroid = new Asteroid(glm::vec3(0.0f, 0.0f, 0.0f), "assets\\rock.obj", "assets\\rock.png");

	m_asteroid->ShaderInfo(m_shader);
	m_asteroid->SetRotSpeed(1);
	m_asteroid->SetSpeed({ -1,-1,-1 });
	m_asteroid->SetScale({ .2,.2,.2 });
	m_asteroid->SetDist({ 10,10,10 });
	m_asteroid->SetRotVector({ 1,1,1 });


	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {
	//getposition locks moon to planet
	//glm vec 3 change location of object
	// 
	//ship
	m_mesh->Update(glm::translate(glm::mat4(1.f), glm::vec3(0,0,0)), dt);
	//haileys comet
	m_asteroid->Update(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), dt);
	//camera
	if(m_camera->isExploration())
		m_camera->Update(m_mesh->GetVectorPos(), m_mesh->GetVectorForward(), m_mesh->GetVectorUp(), dt);
	else
		m_camera->Update(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), dt);

	//solar system
	m_sun->Update(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), dt);

	m_mercury->Update(m_sun->GetPositionMatrix(), dt);
	m_venus->Update(m_sun->GetPositionMatrix(), dt);
	m_earth->Update(m_sun->GetPositionMatrix(), dt);
	m_mars->Update(m_sun->GetPositionMatrix(), dt);
	m_jupiter->Update(m_sun->GetPositionMatrix(), dt);
	m_saturn->Update(m_sun->GetPositionMatrix(), dt);
	m_neptune->Update(m_sun->GetPositionMatrix(), dt);
	m_moon->Update(m_earth->GetPositionMatrix(), dt);
	m_ceres->Update(m_sun->GetPositionMatrix(), dt);
	m_eris->Update(m_sun->GetPositionMatrix(), dt);
	m_haumea->Update(m_sun->GetPositionMatrix(), dt);



	
	m_asteroid->Update(glm::translate(glm::mat4(1.f), glm::vec3(-3, 0, -3)), dt);
	//m_camera->Update(m_mesh->GetVectorPos(), m_mesh->GetVectorForward(), m_mesh->GetVectorUp());
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
	//instancing to make asteroid belt
	m_asteroid->Render(m_shader, m_camera);

	m_mesh->Render(m_shader, m_camera);
	//solar system
	m_sun->Render(m_shader, m_camera);
	m_mercury->Render(m_shader, m_camera);
	m_venus->Render(m_shader, m_camera);
	m_earth->Render(m_shader, m_camera);
	m_mars->Render(m_shader, m_camera);
	m_jupiter->Render(m_shader, m_camera);
	m_saturn->Render(m_shader, m_camera);
	m_neptune->Render(m_shader, m_camera);
	m_moon->Render(m_shader, m_camera);
	m_ceres->Render(m_shader, m_camera);
	m_eris->Render(m_shader, m_camera);
	m_haumea->Render(m_shader, m_camera);
	
	
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

	GLuint globalAmbLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "GlobalAmbient");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("globalAmbient element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), globalAmbLoc, 1, m_light->m_globalAmbient);

	GLuint lightALoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.ambient");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("lightambient element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightALoc, 1, m_light->m_lightAmbient);

	GLuint lightDLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.diffuse");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("lightdiffuse element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightDLoc, 1, m_light->m_lightDiffuse);

	GLuint lightSLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.spec");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("lightspecular element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightSLoc, 1, m_light->m_lightSpecular);

	GLuint lightPosLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.position");
	glProgramUniform3fv(m_shader->GetShaderProgram(), lightSLoc, 1, m_light->m_lightPositionViewSpace);

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

