#include "plane.h"
#include "main.h"

Plane::Plane(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    this->speed = glm::vec3(0, 0, 0.02f);
    this->axis_rotated = glm::mat4(1.0f);

    this->max_height = 100.0f;
    this->height = 1;
    this->width = 8;
    speedy = 0.04;

    this->object = getBlenderObject("plane.obj", "plane.mtl");
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translate = glm::translate (this->position);
    glm::mat4 rotatex    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotatey    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotatez    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));

    axis_rotated *= rotatey * rotatex * rotatez;
    Matrices.model *= (translate * axis_rotated);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    rotation = glm::vec3(0, 0, 0);
}

glm::vec3 Plane::find_relative_pos(glm::vec3 obj_pos) {
    glm::mat4 translate = glm::translate (this->position); 
    glm::vec4 homo_coords = (translate * axis_rotated * glm::vec4(obj_pos.x, obj_pos.y, obj_pos.z, 1));
    return glm::vec3(homo_coords.x, homo_coords.y, homo_coords.z);
}

void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plane::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

