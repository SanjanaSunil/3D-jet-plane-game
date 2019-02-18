#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    glm::mat4 axis_rotated;
    float max_height;
    float height, width;
    void draw(glm::mat4 VP);
    glm::vec3 find_relative_pos(glm::vec3 obj_pos);
    void set_position(float x, float y);
    void tick();
    void accelerate();
    bounding_box_t get_dimensions();
    double speedy;
private:
    VAO *object;
};

#endif 
