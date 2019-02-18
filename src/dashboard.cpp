#include "dashboard.h"
#include "main.h"

Dashboard::Dashboard(float x, float y, float z, float plane_height, float plane_max_height) {
    this->position = glm::vec3(x, y, z);
    this->radius = radius;
    this->rotation = 0;
	
	// Fuel bar
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

	// Altitude bar
	this->altitude_bar_height = 6.2f;
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
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.8f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.8f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;

		altitude_vertex_buffer_data[i++] = -3.55f;
		altitude_vertex_buffer_data[i++] = start_y + count*(float)altitude_bar_height/bar_no - 0.02f;
		altitude_vertex_buffer_data[i++] = 0.0f;
	}
	this->altitude_bar = create3DObject(GL_TRIANGLES, 6+(bar_no+1)*6, altitude_vertex_buffer_data, COLOR_BLACK, GL_FILL);

	// Altitude level
    GLfloat altitude_level_vertex_buffer_data[] = {
	   -3.9f, 0.04f, 0.0f,
       -3.3f, 0.04f, 0.0f,
       -3.3f,-0.04f, 0.0f,
	   -3.9f, 0.04f, 0.0f,
	   -3.3f,-0.04f, 0.0f,
	   -3.9f,-0.04f, 0.0f,
	};
    this->altitude_level = create3DObject(GL_TRIANGLES, 6, altitude_level_vertex_buffer_data, COLOR_RED, GL_FILL);
	this->altitude_level_translate = glm::vec3(0, (plane_height*altitude_bar_height/plane_max_height)-altitude_bar_height/2, 0);

	// Speed - 7 segment display
	GLfloat segment_vertex_buffer_data[18];
	float width = 0.4f, height = 0.04f; 
	GLfloat horizontal_segment_vertex_buffer_data[] = {
	   -width/2, height/2, 0.0f,
       	width/2, height/2, 0.0f,
        width/2,-height/2, 0.0f,
	   -width/2, height/2, 0.0f,
	    width/2,-height/2, 0.0f,
	   -width/2,-height/2, 0.0f,
	};
	GLfloat vertical_segment_vertex_buffer_data[] = {
	   -height/2, width/2, 0.0f,
       	height/2, width/2, 0.0f,
        height/2,-width/2, 0.0f,
	   -height/2, width/2, 0.0f,
	    height/2,-width/2, 0.0f,
	   -height/2,-width/2, 0.0f,
	};
	float segment_x, segment_y;
	for(int i=0; i<7; ++i) 
	{	
		if(i==0) segment_x = 0, segment_y = width - height/2; // top
		if(i==1) segment_x = 0, segment_y = 0; // middle
		if(i==2) segment_x = 0, segment_y = -width + height/2; // bottom
		if(i==3) segment_x = -width/2, segment_y = width/2; // left top
		if(i==4) segment_x = width/2, segment_y = width/2; // right top
		if(i==5) segment_x = -width/2, segment_y = -width/2; // left bottom
		if(i==6) segment_x = width/2, segment_y = -width/2; // right bottom

		if(i<3)
		{
			for(int j=0; j<18; ++j) 
			{	
				segment_vertex_buffer_data[j] = horizontal_segment_vertex_buffer_data[j];
				if((j+1)%3==2) segment_vertex_buffer_data[j] += segment_y;
			}
			this->seven_segment[i] = create3DObject(GL_TRIANGLES, 6, segment_vertex_buffer_data, COLOR_BLACK, GL_FILL);
		}
		else 
		{
			for(int j=0; j<18; ++j) 
			{	
				segment_vertex_buffer_data[j] = vertical_segment_vertex_buffer_data[j];
				if((j+1)%3==1) segment_vertex_buffer_data[j] += segment_x;
				if((j+1)%3==2) segment_vertex_buffer_data[j] += segment_y;
			}
			this->seven_segment[i] = create3DObject(GL_TRIANGLES, 6, segment_vertex_buffer_data, COLOR_BLACK, GL_FILL);
		}		

	}

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

	// Altitude level
	Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->altitude_level_translate);
    Matrices.model *= (translate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->altitude_level);

	// Speed
	float speed_x = 3.5f, speed_y = 3.5f, speed_z = 0.0f;
	for(int i=0; i<3; ++i)
	{
		Matrices.model = glm::mat4(1.0f);
		translate = glm::translate (glm::vec3(speed_x, speed_y, speed_z));
		Matrices.model *= (translate);
    	MVP = VP * Matrices.model;
    	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		for(int j=0; j<7; ++j) draw3DObject(this->seven_segment[j]);

		speed_x -= 0.6f;
	}
}

void Dashboard::reduce_fuel() {
	if(fuel_scale.x>0.0f) fuel_scale.x -= 0.0003f;
}

void Dashboard::set_altitude_level(float plane_height, float plane_max_height) {
	this->altitude_level_translate = glm::vec3(0, (plane_height*altitude_bar_height/plane_max_height)-altitude_bar_height/2, 0);
}

void Dashboard::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Dashboard::tick() {
}

