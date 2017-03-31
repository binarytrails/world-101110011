/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "Terrain.hpp"

Terrain::Terrain(const uint16_t xcells, const uint16_t zcells):
    X_CELLS(xcells), Z_CELLS(zcells), renderMode(GL_TRIANGLES)
{
    this->shader = new Shader("src/shaders/terrain.vs",
                              "src/shaders/terrain.fs");
    this->build();
    this->initBuffers();
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->shader;
}

GLenum Terrain::getRenderMode() const
{
    return this->renderMode;
}

void Terrain::setRenderMode(const GLenum renderMode)
{
    this->renderMode = renderMode;
}

void Terrain::render(const Window* window,
            const Camera* camera,
            const glm::mat4 view,
            const glm::mat4 projection)
{
    this->shader->use();

    this->upload();

    this->updateMVP(view, projection);

    this->draw();
}

void Terrain::rotate(const glm::vec3 spin)
{
    this->model = glm::rotate(this->model, spin.x, glm::vec3(1, 0, 0));
    this->model = glm::rotate(this->model, spin.y, glm::vec3(0, 1, 0));
    this->model = glm::rotate(this->model, spin.z, glm::vec3(0, 0, 1));
}

void Terrain::updateMVP(const glm::mat4 view, const glm::mat4 projection)
{
    // locate in shaders
    GLint modelLoc = glGetUniformLocation(this->shader->programId, "model");
    GLint viewLoc = glGetUniformLocation(this->shader->programId, "view");
    GLint projLoc = glGetUniformLocation(this->shader->programId, "projection");

    // send to shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Terrain::build()
{
    //this->buildPlaneGridRecursive(glm::vec3(-1.0f, 0.5, -1.0f), false);
    this->buildPlaneGrid();
    this->buildPlaneGridIndices();
}

void Terrain::buildPlaneGrid()
{
    for (GLfloat x = -1.0f; x <= 1.0f; x += (1.0f / (this->X_CELLS / 2)))
    {
        for (GLfloat z = -1.0f; z <= 1.0f; z += (1.0f / (this->Z_CELLS / 2)))
        {
            // TODO elevate
            this->vertices.push_back(glm::vec3(x, std::rand() % 4, z));
        }
    }
}

void Terrain::buildPlaneGridRecursive(glm::vec3 v, const bool onetime)
{
    if (v.x > 1.0f || v.z > 1.0f)
        return;
    else
    {
        //printf("push (%f, %f, %f)\n", v.x, v.y, v.z);
        this->vertices.push_back(v);

        v.z += (1.0f / (this->Z_CELLS / 2.0f));
        buildPlaneGridRecursive(v, onetime);

        // v.z coming back in reverse from -1.0f -> 1.0f
        v.x = v.z * -1.0f;
        v.z = -1.0f;

        if (!onetime)
            buildPlaneGridRecursive(v, true);
    }
}

void Terrain::buildPlaneGridIndices()
{
    if (this->getRenderMode() == GL_TRIANGLES)
    {
        for (uint8_t x = 0; x < this->X_CELLS - 1; x++)
        {
            for (uint8_t z = 0; z < this->Z_CELLS - 1; z++)
            {
                uint16_t p1 = z + this->X_CELLS * x;
                uint16_t p2 = z + this->X_CELLS * (x + 1);

                // Triangle 1
                this->verticesI.push_back(p1);
                this->verticesI.push_back(p1 + 1);
                this->verticesI.push_back(p2);
                // Triangle 2
                this->verticesI.push_back(p2);
                this->verticesI.push_back(p1 + 1);
                this->verticesI.push_back(p2 + 1);
            }
        }
    }
    else if (this->getRenderMode() == GL_LINES)
    {
        for (uint8_t x = 0; x < this->X_CELLS - 1; x++)
        {
            for (uint8_t z = 0; z < this->Z_CELLS - 1; z++)
            {
                uint16_t p1 = z + this->X_CELLS * x;
                uint16_t p2 = z + this->X_CELLS * (x + 1);

                // Edge 1
                this->verticesI.push_back(p1);
                this->verticesI.push_back(p1 + 1);

                if (x < this->X_CELLS - 1)
                {
                    // Edge 2
                    this->verticesI.push_back(p1);
                    this->verticesI.push_back(p2);
                }
            }
        }
    }
    else if (this->getRenderMode() == GL_POINTS)
    {
        for (uint8_t x = 0; x < this->X_CELLS; x++)
        {
            for (uint8_t z = 0; z < this->Z_CELLS; z++)
            {
                uint16_t p1 = z + this->X_CELLS * x;
                this->verticesI.push_back(p1);
            }
        }
    }
}

void Terrain::initBuffers()
{
    glGenBuffers(1, &this->vboId);
    glGenVertexArrays(1, &this->vaoId);
    glGenBuffers(1, &this->eboId);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * this->vertices.size(),
                 &this->vertices[0],
                 GL_STATIC_DRAW);

    // has to be before ebo bind
    glBindVertexArray(this->vaoId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(this->verticesI) * this->verticesI.size(),
                 &this->verticesI[0],
                 GL_STATIC_DRAW);

    // enable vao -> vbo pointing
    glEnableVertexAttribArray(0);
    // setup formats of my vao attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind vao
    glBindVertexArray(0);
}

void Terrain::upload()
{
    // TODO send matrices
    //
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(GLushort) * this->verticesI.size(),
                    &this->verticesI[0],
                    GL_STATIC_DRAW);

    // don't disconnect to draw
}

void Terrain::draw()
{
    glBindVertexArray(this->vaoId);
        glDrawElements(this->renderMode,
                       this->verticesI.size(),
                       GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
