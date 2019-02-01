#include "plane.h"
#include "main.h"

Plane::Plane(float x, float y, float height, float width, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->height = height;
    this->width = width;
    this->rotation = 0;
    speed = 1;

    int n = 1000;
    const float PI = 3.14159265359;
    float angle = (2*PI)/(float)n;
    GLfloat vertex_buffer_data[2*9*n+2*9*n];
    GLfloat y1 = width/2, z1 = 0.0f;
	GLfloat y2 = cos(angle)*y1 - sin(angle)*z1;
    GLfloat z2 = sin(angle)*y1 + cos(angle)*z1;

    // Circle and cone
    for(int i=0; i<2*9*n; i+=9) {

        float xpos = height/2;
        if(i>=9*n) xpos *= -1;

        vertex_buffer_data[i] = xpos; //Centre of right circle
        if(i<9*n) vertex_buffer_data[i] += width;
        vertex_buffer_data[i+1] = 0.0f;
		vertex_buffer_data[i+2] = 0.0f;

		vertex_buffer_data[i+3] = xpos;
		vertex_buffer_data[i+4] = y1;
		vertex_buffer_data[i+5] = z1;

		vertex_buffer_data[i+6] = xpos;
		vertex_buffer_data[i+7] = y2;
        vertex_buffer_data[i+8] = z2;

        y1 = y2;
        z1 = z2;

        y2 = cos(angle)*y1 - sin(angle)*z1;
        z2 = sin(angle)*y1 + cos(angle)*z1;
    }

    y1 = width/2, z1 = 0.0f;
	y2 = cos(angle)*y1 - sin(angle)*z1;
    z2 = sin(angle)*y1 + cos(angle)*z1;

    // Cylinder
    for(int i=2*9*n; i<4*9*n; i+=18) {
        float xpos = height/2;

        vertex_buffer_data[i] = xpos;
        vertex_buffer_data[i+1] = y1;
		vertex_buffer_data[i+2] = z1;

		vertex_buffer_data[i+3] = xpos;
		vertex_buffer_data[i+4] = y2;
		vertex_buffer_data[i+5] = z2;

		vertex_buffer_data[i+6] = -1*xpos;
		vertex_buffer_data[i+7] = y1;
        vertex_buffer_data[i+8] = z1;

        vertex_buffer_data[i+9] = -1*xpos;
        vertex_buffer_data[i+10] = y1;
		vertex_buffer_data[i+11] = z1;

		vertex_buffer_data[i+12] = -1*xpos;
		vertex_buffer_data[i+13] = y2;
		vertex_buffer_data[i+14] = z2;

		vertex_buffer_data[i+15] = xpos;
		vertex_buffer_data[i+16] = y2;
        vertex_buffer_data[i+17] = z2;

        y1 = y2;
        z1 = z2;

        y2 = cos(angle)*y1 - sin(angle)*z1;
        z2 = sin(angle)*y1 + cos(angle)*z1;
    }
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->object = create3DObject(GL_TRIANGLES, 12*n, vertex_buffer_data, color, GL_FILL);
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plane::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

