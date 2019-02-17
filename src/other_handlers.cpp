#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

using namespace std;

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow(GLFWwindow *window, int width, int height) {
    int fbwidth = width, fbheight = height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
       is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    // GLfloat fov = 90.0f;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
           glLoadIdentity ();
           gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    reset_screen();
}

struct VAO *getBlenderObject(string objfile, string mtlfile) {

    objfile = "../res/objs/" + objfile;
    mtlfile = "../res/mtls/" + mtlfile;

    string line;

    // Parse mtl file
    ifstream ipmtl(mtlfile);
    map<string, string> mtl_color;
    
    string mtl = "";
    string values = "";
    while(getline(ipmtl, line))
    {
        if(line.substr(0,6)=="newmtl") mtl = line.substr(7);
        if(line.substr(0,3)=="Kd ") values = line.substr(3);

        if(mtl!="" && values!="") 
        {
            mtl_color.insert(pair<string, string>(mtl, values));
            mtl = "";
            values = "";
        }
    }
    
    // Parse obj file
    ifstream ipobj(objfile);

    vector<float> vertices;
    vector<float> coords;
    vector<float> colors;
    while(getline(ipobj, line)) 
    {
        if(line[0]=='v' && line[1]==' ') 
        {
            int len = line.size();
            for(int i=2; i<len; ++i)
            {
                string str_val = "";
                while(line[i]!='\0' && line[i]!=' ')
                {
                    str_val.push_back(line[i]);
                    i++;
                }
                float val = stof(str_val);
                vertices.push_back(val);
            }
            
        }

        if(line.substr(0,6)=="usemtl") values = mtl_color[line.substr(7)];

        if(line[0]=='f' && line[1]==' ')
        {
            int len = line.size();
            for(int i=0; i<len; ++i)
            {
                if(line[i]==' ')
                {
                    i++;
                    string str_val = "";
                    while(line[i]!='/' && i<len) 
                    {
                        str_val.push_back(line[i]);
                        i++;
                    }
                    int val = stoi(str_val);
                    coords.push_back(vertices[val*3-3]);
                    coords.push_back(vertices[val*3-2]);
                    coords.push_back(vertices[val*3-1]);

                    int j = 0;
                    string r = "", g = "", b = "";
                    while(values[j]!=' ') r += values[j++];
                    j++;
                    while(values[j]!=' ') g += values[j++];
                    j++;
                    while(values[j]!='\0') b += values[j++];
                    colors.push_back(stof(r));
                    colors.push_back(stof(g));
                    colors.push_back(stof(b));
                }
            }
        }
    }

    int total_len = (int) coords.size();
    GLfloat vertex_buffer_data[total_len+10];
    GLfloat color_buffer_data[total_len+10];

    for(int i=0; i<total_len; ++i) 
    {
        vertex_buffer_data[i] = coords[i];
        color_buffer_data[i] = colors[i];
    }

    struct VAO* object = create3DObject(GL_TRIANGLES, total_len/3, vertex_buffer_data, color_buffer_data, GL_FILL);
    return object;
}