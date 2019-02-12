#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, float height, float width, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::mat4 axis_rotated;
    float height, width;
    void draw(glm::mat4 VP);
    glm::vec3 find_relative_pos(glm::vec3 obj_pos);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif 
