#include "main.h"
#include "timer.h"
#include "sea.h"
#include "plane.h"
#include "target.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Sea sea;
Plane player;
Target target_point;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

float eye_x, eye_y, eye_z;
float target_x, target_y, target_z;
float up_x, up_y, up_z;

int perspective;

Timer t60(1.0 / 60);

void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    glUseProgram (programID);

    glm::vec3 eye (eye_x, eye_y, eye_z);
    glm::vec3 target (target_x, target_y, target_z);
    glm::vec3 up (0, 1, 0);

    Matrices.view = glm::lookAt( eye, target, up );
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    sea.draw(VP);
    player.draw(VP);

    if(perspective==1) target_point.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);

    int one = glfwGetKey(window, GLFW_KEY_1);
    int two = glfwGetKey(window, GLFW_KEY_2);

    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_S);
    int forward = glfwGetKey(window, GLFW_KEY_W);

    if (up) {
        player.position.y += player.speed;
    }

    if (down) {
        player.position.y -= player.speed;
    }

    if (forward) {
        player.position.z += player.speed;
    }

    // Front view - WILL NEED TO CHANGE ACCORDING TO THE TILT OF PLANE
    if (one) perspective = 1;
    // Tower view
    if (two) perspective = 2;
}

void change_view() {

    if(perspective==1) {
        eye_x = player.position.x;
        eye_y = player.position.y; 
        eye_z = player.position.z + player.height/2 +player.width;
        target_x = player.position.x, target_y = player.position.y, target_z = player.position.z+10;
    }

    if(perspective==2) {
        eye_x = 40, eye_y = 7, eye_z = 40;
        target_x = player.position.x, target_y = player.position.y, target_z = player.position.z;
    }
}

void tick_elements() {
    camera_rotation_angle += 1;

    change_view();
    
    // WILL NEED TO CHANGE ACCORDING TO TILT OF PLANE
    target_point.position.x = player.position.x;
    target_point.position.y = player.position.y;
    target_point.position.z = player.position.z + player.height + player.width;
}


void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    perspective = 1;

    sea       = Sea(0, 0, 0, COLOR_BLUE);

    player    = Plane(0, 0, 0, 4, 2, COLOR_RED);
    eye_x = player.position.x;
    eye_y = player.position.y; 
    eye_z = player.position.z + player.height/2 +player.width;
    target_x = player.position.x, target_y = player.position.y, target_z = player.position.z+10;

    target_point = Target(player.position.x, player.position.y, player.position.z, 0.3, COLOR_BLACK);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
