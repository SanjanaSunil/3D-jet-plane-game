#include "target.h"
#include "main.h"

Target::Target(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->radius = radius;
    this->rotation = 0;

    int n = 1000;
    const float PI = 3.14159265359;
    float angle = (2*PI)/(float)n;
    
    GLfloat vertex_buffer_data[2*9*n];

    GLfloat y1 = radius, z1 = 0.0f;
	GLfloat y2 = cos(angle)*y1 - sin(angle)*z1;
	GLfloat z2 = sin(angle)*y1 + cos(angle)*z1;

    GLfloat inner_y1 = radius - 0.1f, inner_z1 = 0.0f;
	GLfloat inner_y2 = cos(angle)*inner_y1 - sin(angle)*inner_z1;
	GLfloat inner_z2 = sin(angle)*inner_y1 + cos(angle)*inner_z1;

	for(int i=0; i<2*9*n; i+=18) {
		
		vertex_buffer_data[i] = 0.0f;
		vertex_buffer_data[i+1] = inner_y1;
		vertex_buffer_data[i+2] = inner_z1;

		vertex_buffer_data[i+3] = 0.0f;
		vertex_buffer_data[i+4] = y1;
		vertex_buffer_data[i+5] = z1;

		vertex_buffer_data[i+6] = 0.0f;
		vertex_buffer_data[i+7] = y2;
		vertex_buffer_data[i+8] = z2;

        vertex_buffer_data[i+9] = 0.0f;
		vertex_buffer_data[i+10] = inner_y2;
		vertex_buffer_data[i+11] = inner_z2;

		vertex_buffer_data[i+12] = 0.0f;
		vertex_buffer_data[i+13] = y1;
		vertex_buffer_data[i+14] = z1;

		vertex_buffer_data[i+15] = 0.0f;
		vertex_buffer_data[i+16] = y2;
		vertex_buffer_data[i+17] = z2;

		y1 = y2;
		z1 = z2;

		y2 = cos(angle)*y1 - sin(angle)*z1;
        z2 = sin(angle)*y1 + cos(angle)*z1;

        inner_y1 = inner_y2;
		inner_z1 = inner_z2;

		inner_y2 = cos(angle)*inner_y1 - sin(angle)*inner_z1;
        inner_z2 = sin(angle)*inner_y1 + cos(angle)*inner_z1;
	}

    this->object = create3DObject(GL_TRIANGLES, 6*n, vertex_buffer_data, color, GL_FILL);
}

void Target::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Target::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Target::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

