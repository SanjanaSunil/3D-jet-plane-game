#include "main.h"

#ifndef DASHBOARD_H
#define DASHBOARD_H


class Dashboard {
public:
    Dashboard() {}
    Dashboard(float x, float y, float z);
    glm::vec3 position;
    glm::vec3 fuel_scale;
    float radius;
    float rotation;
    void draw(glm::mat4 VP);
    void reduce_fuel();
    void set_position(float x, float y, float z);
    void tick();
private:
    VAO *fuel_bar;
};

#endif