#include "missile.h"
#include "main.h"

Missile::Missile(float x, float y, float z, float height, float width, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->initial_pos = glm::vec3(0, 0, 0);
    this->rotation = 0;
    this->height = height;
    this->width = width;
    this->present = false;
    this->axis_rotated = glm::mat4(1.0f);
    speed = glm::vec3(0, 0, 0);

    int n = 1000;
    const float PI = 3.14159265359;
    float angle = (2*PI)/(float)n;
    GLfloat vertex_buffer_data[2*9*n+2*9*n];

    GLfloat x1 = width/2, y1 = 0.0f;
    GLfloat x2 = cos(angle)*x1 - sin(angle)*y1;
    GLfloat y2 = sin(angle)*x1 + cos(angle)*y1;

    // Circle and cone
    for(int i=0; i<2*9*n; i+=9) {

        float zpos = height/2;
        if(i>=9*n) zpos *= -1;

        vertex_buffer_data[i] = 0.0f; //Centre of right circle
        vertex_buffer_data[i+1] = 0.0f;
        vertex_buffer_data[i+2] = zpos;
        if(i<9*n) vertex_buffer_data[i+2] += width;

        vertex_buffer_data[i+3] = x1;
        vertex_buffer_data[i+4] = y1;
        vertex_buffer_data[i+5] = zpos;

        vertex_buffer_data[i+6] = x2;
        vertex_buffer_data[i+7] = y2;
        vertex_buffer_data[i+8] = zpos;

        x1 = x2;
        y1 = y2;

        x2 = cos(angle)*x1 - sin(angle)*y1;
        y2 = sin(angle)*x1 + cos(angle)*y1;

    }
    

    x1 = width/2, y1 = 0.0f;
    x2 = cos(angle)*x1 - sin(angle)*y1;
    y2 = sin(angle)*x1 + cos(angle)*y1;

    for(int i=2*9*n; i<4*9*n; i+=18) {

        float zpos = height/2;
        vertex_buffer_data[i] = x1;
        vertex_buffer_data[i+1] = y1;
        vertex_buffer_data[i+2] = zpos;

        vertex_buffer_data[i+3] = x2;
        vertex_buffer_data[i+4] = y2;
        vertex_buffer_data[i+5] = zpos;

        vertex_buffer_data[i+6] = x1;
        vertex_buffer_data[i+7] = y1;
        vertex_buffer_data[i+8] = -1*zpos;

        vertex_buffer_data[i+9] = x1;
        vertex_buffer_data[i+10] = y1;
        vertex_buffer_data[i+11] = -1*zpos;

        vertex_buffer_data[i+12] = x2;
        vertex_buffer_data[i+13] = y2;
        vertex_buffer_data[i+14] = -1*zpos;

        vertex_buffer_data[i+15] = x2;
        vertex_buffer_data[i+16] = y2;
        vertex_buffer_data[i+17] = zpos;

        x1 = x2;
        y1 = y2;

        x2 = cos(angle)*x1 - sin(angle)*y1;
        y2 = sin(angle)*x1 + cos(angle)*y1;
    }

    this->object = create3DObject(GL_TRIANGLES, 12*n, vertex_buffer_data, color, GL_FILL);
}

void Missile::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate*axis_rotated);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Missile::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Missile::tick() {
    speed.z += 1.0f;
    glm::mat4 translate = glm::translate (this->initial_pos);
    glm::vec4 homo_coords = (translate * axis_rotated * glm::vec4(0, 0, speed.z, 1));
    position = glm::vec3(homo_coords.x, homo_coords.y, homo_coords.z);

    if(speed.z>=100.0f)
    { 
        this->present = false;
        this->initial_pos = glm::vec3(0, 0, 0);
        this->axis_rotated = glm::mat4(1.0f);
        this->speed = glm::vec3(0, 0, 0);
    }
}

bounding_box_t Missile::get_dimensions() {
    return {position.x, position.y+1, position.z, width, width, height};
}