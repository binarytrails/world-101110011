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
    delete this->elevation;
}

GLenum Terrain::getRenderMode() const
{
    return this->renderMode;
}

void Terrain::setRenderMode(const GLenum renderMode)
{
    this->renderMode = renderMode;
}

void Terrain::onCameraChange(const Camera* camera)
{
    if (camera->view() != this->view)
    {
        //printf("camera moved\n");
        //camera->printView();
    }
    view = camera->view();
}

void Terrain::render(const Window* window,
                     const Camera* camera,
                     const glm::mat4 view,
                     const glm::mat4 projection)
{
    this->shader->use();

    this->onCameraChange(camera);

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
    this->elevation = new TerrainHeight();

    //this->genPlaneVertices();
    //this->genPlaneVerticesRecursive(0, 0);

    this->genTerrainVertices(0, 0, this->X_CELLS, this->Z_CELLS);
    // FIXME
    //this->genTerrainVerticesRecursive(0, 0, this->X_CELLS, this->Z_CELLS);

    this->genVerticesI();
}

void Terrain::genTerrainVertices(
    uint16_t x, uint16_t z, uint16_t max_x, uint16_t max_z)
{
    for (GLfloat x = 0; x < max_x; x+=1)
    {
        for (GLfloat z = 0; z < max_z; z+=1)
        {
            // don't normalize before
            GLfloat y = this->elevation->get(x, z);

            glm::vec3 nv = glm::vec3(x / max_x         * 2 - 1,
                                     y / this->Y_CELLS * 2 - 1,
                                     z / max_z         * 2 - 1);

            printf("terrain : push : (%f, %f, %f)\n", nv.x, nv.y, nv.z);
            this->vertices.push_back(nv);
        }
    }
}

// FIXME
void Terrain::genTerrainVerticesRecursive(
    uint16_t x, uint16_t z, uint16_t max_x, uint16_t max_z)
{
    if (x == max_x && z == max_z)
    {
        return;
    }
    else
    {
        if (z < max_z)
        {
            GLfloat y = this->elevation->get(x, z);

            glm::vec3 nv = glm::vec3(x / max_x         * 2 - 1,
                                     y / this->Y_CELLS * 2 - 1,
                                     z / max_z         * 2 - 1);

            printf("terrain : in   : f(%i, %i, %i, %i)\n", x, z, max_x, max_z);
            printf("terrain : push :  (%f, %f, %f)\n", nv.x, nv.y, nv.z);

            this->vertices.push_back(nv);

            // recur
            genTerrainVerticesRecursive(x, z + 1, max_x, max_z);
        }
        else
        {
            genTerrainVerticesRecursive(x + 1, 0, max_x, max_z);
        }
    }
}

void Terrain::genPlaneVertices()
{
    for (uint16_t x = 0; x < this->X_CELLS; x++)
    {
        for (uint16_t z = 0; z < this->Z_CELLS; z++)
        {
            GLfloat nx = (GLfloat) x / this->X_CELLS * 2 - 1;
            GLfloat nz = (GLfloat) z / this->Z_CELLS * 2 - 1;

            this->vertices.push_back(glm::vec3(nx, 0.0f, nz));
        }
    }
}

void Terrain::genPlaneVerticesRecursive(uint16_t x, uint16_t z)
{
    if (x == this->X_CELLS && z == this->Z_CELLS)
    {
        //printf("return %i, %i\n", x, z);
        return;
    }
    else
    {
        if (z < this->Z_CELLS)
        {
            GLfloat nx = (GLfloat) x / this->X_CELLS * 2 - 1;
            GLfloat nz = (GLfloat) z / this->Z_CELLS * 2 - 1;
            this->vertices.push_back(glm::vec3(nx, 0.0f, nz));

            // recur
            //printf("column recur %i, %i\n", x, z);
            genPlaneVerticesRecursive(x, z + 1);
        }
        else
        {
            //printf("row recur %i, %i\n", x + 1, 0);
            genPlaneVerticesRecursive(x + 1, 0);
        }
    }
}

void Terrain::genVerticesI()
{
    if (this->getRenderMode() == GL_TRIANGLES)
    {
        for (uint16_t x = 0; x < this->X_CELLS - 1; x++)
        {
            for (uint16_t z = 0; z < this->Z_CELLS - 1; z++)
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
        for (uint16_t x = 0; x < this->X_CELLS - 1; x++)
        {
            for (uint16_t z = 0; z < this->Z_CELLS - 1; z++)
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
        for (uint16_t x = 0; x < this->X_CELLS; x++)
        {
            for (uint16_t z = 0; z < this->Z_CELLS; z++)
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
