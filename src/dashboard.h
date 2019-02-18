#include "main.h"

#ifndef DASHBOARD_H
#define DASHBOARD_H


class Dashboard {
public:
    Dashboard() {}
    Dashboard(float x, float y, float z, float plane_height, float plane_max_height);
    glm::vec3 position;
    glm::vec3 fuel_scale;
    glm::vec3 altitude_level_translate;
    float altitude_bar_height;
    float radius;
    int speed;
    float rotation;
    void draw(glm::mat4 VP);
    void reduce_fuel();
    void set_position(float x, float y, float z);
    void set_altitude_level(float plane_height, float plane_max_height);
    void set_speed(float plane_speed, float plane_max_speed);
    void tick();
private:
    VAO *fuel_bar;
    VAO *altitude_bar;
    VAO *altitude_level;
    VAO *seven_segment[7];
};

#endif