#include "main.h"

#ifndef BOMB_H
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y, float z);
    glm::vec3 position;
    float speedy, speedx;
    bool present;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    void shoot();
    bounding_box_t get_dimensions();
private:
    VAO *object;
};

#endif
