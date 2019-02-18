#include "island.h"
#include "main.h"

Island::Island(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->object = getBlenderObject("waterfall.obj", "waterfall.mtl");
}

void Island::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    glm::mat4 scale = glm::scale(glm::vec3(2, 2, 2));
    Matrices.model *= translate * scale;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Island::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

