#include "checkpoint.h"
#include "main.h"

Checkpoint::Checkpoint(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->object = getBlenderObject("checkpoint.obj", "checkpoint.mtl");

    this->arrow_position = glm::vec3(x, 7, z);
    this->arrow = getBlenderObject("arrow.obj", "arrow.mtl");
}

void Checkpoint::draw(glm::mat4 VP) {

    // Draw cannon
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    // Draw arrow
    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->arrow_position);
    Matrices.model *= translate;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->arrow);
}

void Checkpoint::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Checkpoint::tick(float plane_height) {
    this->arrow_position = this->position;
    this->arrow_position.y = plane_height;
    if(this->arrow_position.y<7) this->arrow_position.y = 7;
}

bounding_box_t Checkpoint::get_dimensions() {
    return {position.x, position.y+1, position.z, 7.0f, 7.0f, 7.0f};
}
