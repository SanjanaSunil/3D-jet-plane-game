#include "main.h"
#include "timer.h"
#include "sea.h"
#include "plane.h"
#include "dashboard.h"
#include "target.h"

using namespace std;

GLMatrices Matrices;
GLMatrices FixedMatrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Sea sea;
Plane player;
Dashboard dashboard;
Target target_point;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

glm::vec3 cam_eye;
glm::vec3 cam_target;
glm::vec3 cam_up;

int perspective;

Timer t60(1.0 / 60);

void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    glUseProgram (programID);

    FixedMatrices.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    Matrices.view = glm::lookAt( cam_eye, cam_target, cam_up );
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    glm::mat4 FVP = FixedMatrices.projection * FixedMatrices.view;
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    sea.draw(VP);
    player.draw(VP);

    // if(perspective==1) target_point.draw(VP, player.axis_rotated);
    dashboard.draw(FVP);
}

void tick_input(GLFWwindow *window) {

    int one = glfwGetKey(window, GLFW_KEY_1);
    int two = glfwGetKey(window, GLFW_KEY_2);

    int up = glfwGetKey(window, GLFW_KEY_W);
    int down = glfwGetKey(window, GLFW_KEY_S);
    int forward = glfwGetKey(window, GLFW_KEY_SPACE);
    int tilt_left  = glfwGetKey(window, GLFW_KEY_A);
    int tilt_right = glfwGetKey(window, GLFW_KEY_D);
    int rotate_left  = glfwGetKey(window, GLFW_KEY_Q);
    int rotate_right = glfwGetKey(window, GLFW_KEY_E);

    if (up) player.position.y += player.speedy;
    if (down) player.position.y -= player.speedy;
    if (tilt_left) player.rotation.z = -1;
    if (tilt_right) player.rotation.z = 1;
    if (rotate_left) player.rotation.y = -1;
    if (rotate_right) player.rotation.y = 1;
    if (forward) 
    {
        player.position = player.find_relative_pos(player.speed);
        dashboard.reduce_fuel();
    }

    // Front view
    if (one) perspective = 1;
    // Tower view
    if (two) perspective = 2;
}

void tick_elements() {
    camera_rotation_angle += 1;

    target_point.position = player.find_relative_pos(glm::vec3(0, 0, player.height+player.width));

    if(perspective==1) {
        cam_eye = player.find_relative_pos(glm::vec3(0, 0, player.width));
        cam_target = target_point.position;
        cam_up = player.find_relative_pos(glm::vec3(0, 1, 0)) - player.position;
    }

    if(perspective==2) {
        cam_eye = glm::vec3(20, 7, 20);
        cam_target = player.position;
        cam_up = glm::vec3(0, 1, 0);
    }

    // Check altitude
    dashboard.set_altitude_level(player.position.y);

    // Check for death
    if(dashboard.fuel_scale.x<=0.0f) quit(window); //Fuel check
}


void initGL(GLFWwindow *window, int width, int height) {

    glfwSetWindowTitle(window, "Link - Tales of the Jetplane");

    perspective = 1;

    sea       = Sea(0, 0, 0, COLOR_BLUE);

    player    = Plane(0, 5, 0, COLOR_RED);
    cam_eye = glm::vec3(0, 0, player.width);
    cam_target = glm::vec3(0, 0, player.position.z+10);
    cam_up = glm::vec3(0, 1, 0);

    dashboard = Dashboard(0, 0, 0, player.position.y);
    target_point = Target(0, 0, player.height+player.width, 0.2, COLOR_BLACK);

    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    FixedMatrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);

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
    Matrices.projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 500.0f);
}