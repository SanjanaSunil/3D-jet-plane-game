#include "target.h"
#include "main.h"

Target::Target(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->radius = radius;
    this->rotation = 0;

    int n = 1000;
    const float PI = 3.14159265359;
    float angle = (2*PI)/(float)n;
    
    GLfloat vertex_buffer_data[2*9*n+2*9+2*9];

    GLfloat x1 = radius, y1 = 0.0f;
	GLfloat x2 = cos(angle)*x1 - sin(angle)*y1;
	GLfloat y2 = sin(angle)*x1 + cos(angle)*y1;

    GLfloat inner_x1 = radius - 0.1f, inner_y1 = 0.0f;
	GLfloat inner_x2 = cos(angle)*inner_x1 - sin(angle)*inner_y1;
	GLfloat inner_y2 = sin(angle)*inner_x1 + cos(angle)*inner_y1;

	for(int i=0; i<2*9*n; i+=18) {
		
		vertex_buffer_data[i] = inner_x1;
		vertex_buffer_data[i+1] = inner_y1;
		vertex_buffer_data[i+2] = 0.0f;

		vertex_buffer_data[i+3] = x1;
		vertex_buffer_data[i+4] = y1;
		vertex_buffer_data[i+5] = 0.0f;

		vertex_buffer_data[i+6] = x2;
		vertex_buffer_data[i+7] = y2;
		vertex_buffer_data[i+8] = 0.0f;

        vertex_buffer_data[i+9] = inner_x2;
		vertex_buffer_data[i+10] = inner_y2;
		vertex_buffer_data[i+11] = 0.0f;

		vertex_buffer_data[i+12] = x1;
		vertex_buffer_data[i+13] = y1;
		vertex_buffer_data[i+14] = 0.0f;

		vertex_buffer_data[i+15] = x2;
		vertex_buffer_data[i+16] = y2;
		vertex_buffer_data[i+17] = 0.0f;

		x1 = x2;
		y1 = y2;

		x2 = cos(angle)*x1 - sin(angle)*y1;
        y2 = sin(angle)*x1 + cos(angle)*y1;

        inner_x1 = inner_x2;
		inner_y1 = inner_y2;

		inner_x2 = cos(angle)*inner_x1 - sin(angle)*inner_y1;
		inner_y2 = sin(angle)*inner_x1 + cos(angle)*inner_y1;
	}

	// Vertical line
	vertex_buffer_data[2*9*n] = radius + 0.3f;
	vertex_buffer_data[2*9*n+1] = 0.02f;
	vertex_buffer_data[2*9*n+2] = 0.0f;
	vertex_buffer_data[2*9*n+3] = radius + 0.3f;
	vertex_buffer_data[2*9*n+4] = -0.02f;
	vertex_buffer_data[2*9*n+5] = 0.0f;
	vertex_buffer_data[2*9*n+6] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+7] = 0.02f;
	vertex_buffer_data[2*9*n+8] = 0.0f;

	vertex_buffer_data[2*9*n+9] = radius + 0.3f;
	vertex_buffer_data[2*9*n+10] = -0.02f;
	vertex_buffer_data[2*9*n+11] = 0.0f;
	vertex_buffer_data[2*9*n+12] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+13] = -0.02f;
	vertex_buffer_data[2*9*n+14] = 0.0f;
	vertex_buffer_data[2*9*n+15] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+16] = 0.02f;
	vertex_buffer_data[2*9*n+17] = 0.0f;

	// Horizontal line
	vertex_buffer_data[2*9*n+18] = 0.02f;
	vertex_buffer_data[2*9*n+19] = radius + 0.3f;
	vertex_buffer_data[2*9*n+20] = 0.0f;
	vertex_buffer_data[2*9*n+21] = -0.02f;
	vertex_buffer_data[2*9*n+22] = radius + 0.3f;
	vertex_buffer_data[2*9*n+23] = 0.0f;
	vertex_buffer_data[2*9*n+24] = 0.02f;
	vertex_buffer_data[2*9*n+25] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+26] = 0.0f;

	vertex_buffer_data[2*9*n+27] = -0.02f;
	vertex_buffer_data[2*9*n+28] = radius + 0.3f;
	vertex_buffer_data[2*9*n+29] = 0.0f;
	vertex_buffer_data[2*9*n+30] = -0.02f;
	vertex_buffer_data[2*9*n+31] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+32] = 0.0f;
	vertex_buffer_data[2*9*n+33] = 0.02f;
	vertex_buffer_data[2*9*n+34] = -1*radius - 0.3f;
	vertex_buffer_data[2*9*n+35] = 0.0f;

    this->object = create3DObject(GL_TRIANGLES, 6*n+6+6, vertex_buffer_data, color, GL_FILL);
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

