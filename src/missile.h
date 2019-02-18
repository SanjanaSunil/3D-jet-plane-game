#include "main.h"

#ifndef MISSILE_H
#define MISSILE_H


class Missile {
public:
    Missile() {}
    Missile(float x, float y, float z, float height, float width, color_t color);
    glm::vec3 position;
    glm::vec3 initial_pos;
    glm::vec3 speed;
    glm::mat4 axis_rotated;
    float rotation;
    bool present;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    float height, width;
private:
    VAO *object;
};

#endif
