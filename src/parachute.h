#include "main.h"

#ifndef PARACHUTE_H
#define PARACHUTE_H


class Parachute {
public:
    Parachute() {}
    Parachute(float x, float y, float z);
    glm::vec3 position;
    bool present;
    int counter;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    void fall();
    bounding_box_t get_dimensions();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
