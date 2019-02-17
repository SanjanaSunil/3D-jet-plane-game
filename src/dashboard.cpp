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

	float height = 6.2f;
	int bar_no = 10;
	GLfloat altitude_vertex_buffer_data[18+(bar_no+1)*18];
	for(int i=0; i<18; ++i) // Backbone
	{
		if((i+1)%3==0) altitude_vertex_buffer_data[i] = 0.0f;

		if((i+1)%3==2) 
		{
			altitude_vertex_buffer_data[i] = 3.1f;
			if(i==7 || i==13 || i==16) altitude_vertex_buffer_data[i] *= -1;
		}

		if((i+1)%3==1) 
		{
			altitude_vertex_buffer_data[i] = -3.7f;
			if(i==3 || i==6 || i==12) altitude_vertex_buffer_data[i] = -3.65f;
		}
	}
	float start_y = -3.1f;
	int i = 18;
	for(int count=0; count<=bar_no; ++count) 
	{
		altitude_vertex_buffer_data[i++] = -3.8f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.8f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.8f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;
	}
	this->altitude_bar = create3DObject(GL_TRIANGLES, 6+(bar_no+1)*6, altitude_vertex_buffer_data, COLOR_BLACK, GL_FILL);
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

