#include "sea.h"
#include "main.h"

Sea::Sea(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);

    // Sea is on X-Z plane
    static const GLfloat vertex_buffer_data[] = {
         1000.0f, 0.0f, 1000.0f, // triangle 1 : begin
        -1000.0f, 0.0f, 1000.0f,
         1000.0f, 0.0f,-1000.0f, // triangle 1 : end
        -1000.0f, 0.0f,-1000.0f, // triangle 2 : begin
        -1000.0f, 0.0f, 1000.0f,
         1000.0f, 0.0f,-1000.0f, // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Sea::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Sea::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

