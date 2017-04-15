/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "Terrain.hpp"

Terrain::Terrain(const uint16_t xcells, const uint16_t zcells):
    X_CELLS(xcells), Z_CELLS(zcells), renderMode(GL_TRIANGLES),
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
        this->texFilepath.c_str(), &w, &h, 0, SOIL_LOAD_RGB);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, image);

        glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind
}

void Terrain::build()
{
    this->elevation = new TerrainHeight();

    // Note: size*2 because each vertex has a texture

    //this->genPlaneVertices();
    //this->genPlaneVerticesRecursive(0, 0);

    //this->genTerrainVertices(0, 0, this->X_CELLS, this->Z_CELLS);
    this->genTerrainVerticesRecursive(0, 0, this->X_CELLS, this->Z_CELLS);

    this->genVerticesI();
}

// by directions steps
void Terrain::advance(const glm::ivec3 ds)
{
    /* TODO implement
     *
     */
    std::vector<glm::vec3> vx = this->vertices;

    this->vertices.clear();
    this->verticesI.clear();

    // TODO do not regenerate entirely but only in step size
    this->genTerrainVerticesRecursive(
            ds.x,                 ds.z,
            this->X_CELLS + ds.x, this->Z_CELLS + ds.z
    );
    this->genVerticesI();

    this->upload();

    printf("terrain : advance(%i, %i, %i)\n", ds.x, ds.y, ds.z);
}

// Note: takes only positive range
void Terrain::genTerrainVertices(
    uint16_t x, uint16_t z, uint16_t max_x, uint16_t max_z)
{
    for (GLfloat x = 0; x < max_x; x+=1)
    {
        for (GLfloat z = 0; z < max_z; z+=1)
        {
            glm::vec3 v(x, this->elevation->get(x, z), z);
            printf("terrain : push : vertex(%f, %f, %f)\n", v.x, v.y, v.z);
            this->vertices.push_back(v);
        }
    }
}

void Terrain::genTerrainVerticesRecursive(
    uint16_t x, uint16_t z, uint16_t max_x, uint16_t max_z)
{
    if (x == max_x && z == max_z)
    {
        glm::vec3 v(x, this->elevation->get(x, z), z);
        printf("terrain : push : vertex(%f, %f, %f)\n", v.x, v.y, v.z);
        this->vertices.push_back(v);
        return;
    }
    else
    {
        if (z < max_z)
        {
            glm::vec3 v(x, this->elevation->get(x, z), z);
            printf("terrain : push : vertex(%f, %f, %f)\n", v.x, v.y, v.z);
            this->vertices.push_back(v);

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
            printf("terrain : push : vertex(%i, %i, %i)\n", x, 0, z * -1);
            this->vertices.push_back(glm::vec3(x, 0, z));
        }
    }
}

void Terrain::genPlaneVerticesRecursive(uint16_t x, uint16_t z)
{
    if (x == this->X_CELLS && z == this->Z_CELLS)
    {
        printf("terrain : push : vertex(%i, %i, %i)\n", x, 0, z);
        this->vertices.push_back(glm::vec3(x, 0, z));
        return;
    }
    else
    {
        if (z < this->Z_CELLS)
        {
            printf("terrain : push : vertex(%i, %i, %i)\n", x, 0, z);
            this->vertices.push_back(glm::vec3(x, 0, z));

            // recur
            genPlaneVerticesRecursive(x, z + 1);
        }
        else
        {
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
    // TODO resend vertices for advancing (infinite terrain)

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(GLushort) * this->verticesI.size(),
                    &this->verticesI[0],
                    GL_STATIC_DRAW);

    // don't disconnect to draw
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
