#include "ring.h"
#include "main.h"

Ring::Ring(float x, float y, float z) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    this->present = false;

    this->object = getBlenderObject("ring.obj", "ring.mtl");
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ring::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Ring::get_dimensions() {
    return {position.x, position.y, position.z, 6.0f, 6.0f, 6.0f};
}
