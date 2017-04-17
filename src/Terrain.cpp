/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "Terrain.hpp"

Terrain::Terrain(const uint16_t xcells, const uint16_t zcells):
    X_CELLS(xcells), Z_CELLS(zcells), x_offset(0), z_offset(0),
    renderMode(GL_TRIANGLES),
    texFilepath("assets/textures/grass_green_d.jpg")
{
    this->shader = new Shader("src/shaders/terrain.vs",
                              "src/shaders/terrain.fs");
    this->loadTexture();
    this->build();

    glPointSize(3);

    this->initBuffers();
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->shader;
    delete this->elevation;
}

float Terrain::getElevation(const float x, const float z)
{
    glm::vec3 vertex = this->vertices[(uint16_t) x * this->X_CELLS +
                                      (uint16_t) z * this->Z_CELLS];
    return vertex.y;
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

void Terrain::loadTexture()
{
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h;
    unsigned char* image = SOIL_load_image(
        this->texFilepath.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image);

        glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind
}

void Terrain::build()
{
    this->elevation = new TerrainHeight();

    //this->genPlaneVertices(0, 0);
    this->genTerrainVertices(0, 0, this->X_CELLS, this->Z_CELLS);

    this->genVerticesI();
}

void Terrain::addVertex(const float _x, const float _z, const bool elevate)
{
    glm::vec3 v(_x, 0, _z);

    if (elevate)
    {
        // this->elevate->setAmplitude()
        v.y = this->elevation->get(v.x, v.z);
    }

    printf("terrain : push : vertex(%f, %f, %f)\n", v.x, v.y, v.z);
    this->vertices.push_back(v);
}

// curently one at the time
bool Terrain::advance(const bool forward)
{
    // TODO work in progress;
    return false;

    this->x_offset += (forward) ? 1 : -1;
    //this->x_offset = 0;
    this->z_offset += (forward) ? 1 : -1;

    if (this->X_CELLS + this->x_offset <= 0 ||
        this->Z_CELLS + this->z_offset <= 0)
        return false;

    printf("\nterrain : advance");//(%i, %i)\n", x_steps, z_steps);
    printf("terrain : vertices.size()->%i\n", this->vertices.size());

    std::vector<glm::vec3> vbuffer(this->vertices);

    for (uint16_t z = 0; z < this->Z_CELLS; z++)
    {
        uint16_t index = (this->X_CELLS - x_offset) * this->Z_CELLS + z;

        glm::vec3 cell = vbuffer[index];

        printf("terrain : advance : bcell (%f, %f, %f)\n", cell.x, cell.y, cell.z);

        float _x = cell.x + this->x_offset;
        float _z = cell.z + this->z_offset;


        // clamp em
        if (_x > this->X_CELLS)
            _x = this->X_CELLS - fmod(_x, this->X_CELLS);

        if (_z > this->Z_CELLS)
            _z = this->Z_CELLS - fmod(_z, this->Z_CELLS);


        printf("terrain : advance : elevation->get(%f, %f)\n", _x, _z);
        cell.y = this->elevation->get(_x, _z);

        printf("terrain : advance : acell (%f, %f, %f)\n\n", cell.x, cell.y, cell.z);

        vbuffer[index] = cell;
    }

    assert(vbuffer.size() == this->vertices.size());

    //vbuffer.clear();

    printf("\nterrain : x_offset (%i)\n", this->x_offset);
    printf("terrain : z_offset (%i)\n\n", this->z_offset);

    // erase current
    this->vertices.clear();
    this->verticesI.clear();
    this->upload();

    // load new
    this->vertices = vbuffer;
    this->genVerticesI();
    this->upload();

    printf("terrain : vertices.size()->%i\n", this->vertices.size());
    return true;
}

void Terrain::genTerrainVertices(uint16_t x, uint16_t z,
                                 uint16_t max_x, uint16_t max_z)
{
    if (x == max_x && z == max_z)
    {
        this->addVertex(x, z, true);
        return;
    }
    else
    {
        if (z < max_z)
        {
            this->addVertex(x, z, true);
            genTerrainVertices(x, z + 1, max_x, max_z);
        }
        else
        {
            genTerrainVertices(x + 1, 0, max_x, max_z);
        }
    }
}

void Terrain::genPlaneVertices(uint16_t x, uint16_t z)
{
    if (x == this->X_CELLS && z == this->Z_CELLS)
    {
        this->addVertex(x, z, false);
        return;
    }
    else
    {
        if (z < this->Z_CELLS)
        {
            this->addVertex(x, z, false);
            genPlaneVertices(x, z + 1);
        }
        else
        {
            genPlaneVertices(x + 1, 0);
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

    // position indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(this->verticesI[0]) * this->verticesI.size(),
                 &this->verticesI[0],
                 GL_STATIC_DRAW);

    // enable vao -> vbo pointing
    glEnableVertexAttribArray(0);

    // setup formats of my vao attributes
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
    // texture coordinates attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

    glEnableVertexAttribArray(1);

    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind vao
    glBindVertexArray(0);
}

void Terrain::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

        //glBufferData(GL_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW);

        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(glm::vec3) * this->vertices.size(),
                     &this->vertices[0],
                     GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(this->vaoId);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);

            //glBufferData(GL_ELEMENT_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(GLushort) * this->verticesI.size(),
                        &this->verticesI[0],
                        GL_STATIC_DRAW);

        // don't disconnect to draw
    glBindVertexArray(0);
}

void Terrain::draw()
{
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glBindVertexArray(this->vaoId);

        glDrawElements(this->renderMode,
                       this->verticesI.size(),
                       GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}
