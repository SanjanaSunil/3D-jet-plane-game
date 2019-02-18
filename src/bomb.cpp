#include "bomb.h"
#include "main.h"

Bomb::Bomb(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->present = false;
    speed = 0.08f;
    this->object = getBlenderObject("bomb.obj", "bomb.mtl");
}

void Bomb::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Bomb::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Bomb::tick() {
    this->position.y -= speed;
    if(this->position.y<=0) this->present = false;
}

