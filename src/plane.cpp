#include "plane.h"
#include "main.h"

Plane::Plane(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    this->speed = glm::vec3(0, 0, 0);
    this->axis_rotated = glm::mat4(1.0f);

    this->score = 0;
    this->max_height = 50.0f;
    this->height = 1;
    this->width = 7.5;
    this->max_speed = 0.2f;
    speedy = 0.04;

    this->object = getBlenderObject("plane.obj", "plane.mtl");
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);

    // glm::mat4 scale = glm::scale(glm::vec3(0.8, 0.8, 0.8));
    glm::mat4 rotatex = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotatey = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotatez = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 translate = glm::translate (this->position);

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

bounding_box_t Plane::get_dimensions() {
    return {position.x, position.y, position.z, 3.0f, 3.0f, 8.0f};
}

void Plane::tick() {
    this->position = this->find_relative_pos(this->speed);
    if(this->speed.z>0) this->speed.z -= 0.0005f;
    if(this->speed.z<0) this->speed.z = 0;
}

void Plane::accelerate() {
    this->speed.z += 0.001f;
    if(this->speed.z>=max_speed) this->speed.z = max_speed;
}

