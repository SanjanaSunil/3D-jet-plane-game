#include "dashboard.h"
#include "main.h"

Dashboard::Dashboard(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->radius = radius;
    this->rotation = 0;
	
	this->fuel_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    GLfloat fuel_vertex_buffer_data[] = {
	    3.8f,-3.5f, 0.0f,
        3.8f,-3.8f, 0.0f,
       -3.8f,-3.5f, 0.0f,
       -3.8f,-3.5f, 0.0f,
        3.8f,-3.8f, 0.0f,
	   -3.8f,-3.8f, 0.0f
	};
    this->fuel_bar = create3DObject(GL_TRIANGLES, 6, fuel_vertex_buffer_data, COLOR_GREEN, GL_FILL);

	GLfloat altitude_vertex_buffer_data[] = {
		-3.8f, 3.1f, 0.0f,
		-3.7f, 3.1f, 0.0f,
		-3.7f,-3.1f, 0.0f,
		-3.8f, 3.1f, 0.0f,
		-3.7f,-3.1f, 0.0f,
		-3.8f,-3.1f, 0.0f
	};
	this->altitude_bar = create3DObject(GL_TRIANGLES, 6, altitude_vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Dashboard::draw(glm::mat4 VP) {

	// Fuel bar
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
	glm::mat4 scale = glm::scale (fuel_scale);
    Matrices.model *= (translate*scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->fuel_bar);

	// Altitude bar
	Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    Matrices.model *= (translate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->altitude_bar);
}

void Dashboard::reduce_fuel() {
	if(fuel_scale.x>0.0f) fuel_scale.x -= 0.0003f;
}

void Dashboard::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Dashboard::tick() {
}

