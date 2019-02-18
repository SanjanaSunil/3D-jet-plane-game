#include "main.h"

#ifndef CHECKPOINT_H
#define CHECKPOINT_H


class Checkpoint {
public:
    Checkpoint() {}
    Checkpoint(float x, float y, float z);
    glm::vec3 position;
    glm::vec3 arrow_position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float plane_height);
    bounding_box_t get_dimensions();
private:
    VAO *object;
    VAO *arrow;
};

#endif
