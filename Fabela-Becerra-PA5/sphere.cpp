#include "sphere.h"

Sphere::Sphere()
{
    init(48);
    setupVertices();
    setupBuffers();
    speed = { 0.0, 0.0, 0.0 };
    dist = { 0.0, 0.0, 0.0 };
    rotSpeed = 0;
    rotVector = glm::vec3(0, 0, 0);
    scale = { 1, 1, 1 };
}

Sphere::Sphere(Sphere* otherSphere, glm::vec3 startAngle)
{
    Vertices = otherSphere->Vertices;
    Indices = otherSphere->Indices;
    numIndices = otherSphere->numIndices;
    m_texture = otherSphere->m_texture;
    m_normalTexture = otherSphere->m_normalTexture;
    hasTexture = otherSphere->hasTexture;
    hasNormal = otherSphere->hasNormal;
    setupBuffers();

    angle = startAngle;
    speed = { 0.0, 0.0, 0.0 };
    dist = { 0.0, 0.0, 0.0 };
    rotSpeed = 0;
    rotVector = glm::vec3(0, 0, 0);
    scale = { 1, 1, 1 };
}

Sphere::Sphere(const int prec, const char* fname, const char* nname) {
    // prec is precision, or number of slices
    init(prec);
    setupVertices();
    setupBuffers();

    // load texture from file
    m_texture = new Texture(fname, true);
    if (m_texture)
        hasTexture = true;
    else
        hasTexture = false;

    m_normalTexture = new Texture(nname, false);
    if (m_normalTexture)
        hasNormal = true;
    else
        hasNormal = false;

    
    speed = { 0.0, 0.0, 0.0 };
    dist = { 0.0, 0.0, 0.0 };
    rotSpeed = 0;
    rotVector = glm::vec3(0, 0, 0);
    scale = { 1, 1, 1 };
}

void Sphere::setupVertices() {
    const std::vector<int> ind = getIndices();
    const std::vector<glm::vec3> vert = getVertices();
    const std::vector<glm::vec2> tex = getTexCoords();
    const std::vector<glm::vec3> norm = getNormals();


    const int numIndices = getNumIndices();
    for (int i = 0; i < numIndices; i++) {
        Vertices.emplace_back(glm::vec3(vert[ind[i]].x, vert[ind[i]].y, vert[ind[i]].z),
            glm::vec3(norm[ind[i]].x, norm[ind[i]].y, norm[ind[i]].z),
            glm::vec2(tex[ind[i]].x, tex[ind[i]].y));
        Indices.push_back(i);
    }
}

void Sphere::setupBuffers() {
    // For OpenGL 3
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), (Vertices).data(), GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), (Indices).data(), GL_STATIC_DRAW);
}

void Sphere::SetRotVector(glm::vec3 newRotVector)
{
    posVertex = speed[0] != 0.0 ?
        glm::vec3(cos(speed[0] * angle.x) * dist[0], sin(speed[1] * angle.y) * dist[1], sin(speed[2] * angle.z) * dist[2]) :
        glm::vec3(cos(speed[0] * angle.x) * dist[0], cos(speed[1] * angle.y) * dist[1], sin(speed[2] * angle.z) * dist[2]);
    rotVector = newRotVector;
}

void Sphere::Update(glm::mat4 matModel, double dt)
{
    lastFrame = (dt);
    posVertex = speed[0] != 0.0 ?
        glm::vec3(cos(speed[0] * lastFrame) * dist[0], sin(speed[1] * lastFrame) * dist[1], sin(speed[2] * lastFrame) * dist[2]) :
        glm::vec3(cos(speed[0] * lastFrame) * dist[0], cos(speed[1] * lastFrame) * dist[1], sin(speed[2] * lastFrame) * dist[2]);
    matModel *= glm::translate(glm::mat4(1.f), posVertex);
    positionMatrix = matModel;
    matModel *= glm::rotate(glm::mat4(1.f), rotSpeed * static_cast<float>(dt), rotVector);
    matModel *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
    model = matModel;
}

void Sphere::Render(Shader* m_shader, Camera* m_camera)
{
    // Start the correct program
    m_shader->Enable();

    // Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
    glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * GetModel())))));
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

    GLuint mAmbLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.ambient");
    glProgramUniform4fv(m_shader->GetShaderProgram(), mAmbLoc, 1, matAmbient);

    GLuint mDiffLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.diffuse");
    glProgramUniform4fv(m_shader->GetShaderProgram(), mDiffLoc, 1, matDiff);

    GLuint mSpecLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.spec");
    glProgramUniform4fv(m_shader->GetShaderProgram(), mSpecLoc, 1, matSpec);

    GLuint mShineLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.shininess");
    glProgramUniform1f(m_shader->GetShaderProgram(), mShineLoc, matShininess);

    GLuint mViewPos = glGetUniformLocation(m_shader->GetShaderProgram(), "viewPos");
    glProgramUniform3f(m_shader->GetShaderProgram(), mViewPos, m_camera->cameraPos.x, m_camera->cameraPos.y, m_camera->cameraPos.z);

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
    glDrawElements(GL_TRIANGLES, getNumIndices(), GL_UNSIGNED_INT, nullptr);

    // Disable vertex arrays
    glDisableVertexAttribArray(m_positionAttrib);
    glDisableVertexAttribArray(m_colorAttrib);
    glDisableVertexAttribArray(m_tcAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphere::SetDist(std::vector<double> newDist)
{
    posVertex = glm::vec3(newDist[0], newDist[1], newDist[2]);
    dist = newDist;
}

void Sphere::init(int prec)
{
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { texCoords.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { normals.emplace_back(); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            const auto y = (float)cos(toRadians(180.f - i * 180.f / prec));
            const auto x = -(float)cos(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            const float z = (float)sin(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2((static_cast<float>(j) / prec), (static_cast<float>(i) / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangles indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

bool Sphere::ShaderInfo(Shader* shader)
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

    // Locate the normal matrix in the shader
    m_normalMatrix = shader->GetUniformLocation("normMatrix");
    if (m_normalMatrix == INVALID_UNIFORM_LOCATION)
    {
        printf("m_normalMatrix not found\n");
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