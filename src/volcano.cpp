#include "volcano.h"
#include "main.h"

Volcano::Volcano(float x, float y, float z) {
    this->scale_factor = rand()%2 + 2;
    this->position = glm::vec3(x, y, z);
    this->object = getBlenderObject("volcano.obj", "volcano.mtl");
}

void Volcano::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 scale = glm::scale(glm::vec3(scale_factor, scale_factor, scale_factor));
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= translate * scale;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Volcano::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

bounding_box_t Volcano::get_dimensions() {
    return {position.x, position.y, position.z, 10.0f, 30.0f, 10.0f};
}