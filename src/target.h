#include "main.h"

#ifndef TARGET_H
#define TARGET_H


class Target {
public:
    Target() {}
    Target(float x, float y, float z, float radius, color_t color);
    glm::vec3 position;
    float radius;
    float rotation;
    void draw(glm::mat4 VP, glm::mat4 rotate);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif
