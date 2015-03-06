
#include <iostream>
#include <cmath>



#define GLEW_STATIC
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>
#undef GLFW_DLL
#include <GLFW/glfw3.h>
#include "HalfPipe.h"
#include "GenericObject2.h"

using namespace std;

HalfPipe pipe;
GenericObject2 base;
void win_refresh(GLFWwindow*);
float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;
glm::mat4 camera_cf; // {glm::translate(glm::mat4(1.0f), glm::vec3{0,0,-5})};

void err_function (int what, const char *msg) {
    cerr << what << " " << msg << endl;
}
void win_resize (GLFWwindow * win, int width, int height)
{
#ifdef DEBUG
    cout << __FUNCTION__ << " " << width << "x" << height << endl;
#endif
    int w, h;
    glfwGetWindowSize(win, &w, &h);
    screen_ctr_x = w / 2.0;
    screen_ctr_y = h / 2.0;
    float rad = min(h,w)/2;
    arc_ball_rad_square = rad * rad;
    /* Use the entire window for our view port */
    glViewport(0, 0, width, height);
    /* Use GL_PROJECTION to select the type of synthetic camera */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* near-plane(1) & far-plane(10) are always POSITIVE and they measure
     * the distances along the Z-axis in front of the camera */
    gluPerspective(60.0, static_cast<float> (width)/ static_cast<float> (height), 1, 100);
}

void win_refresh (GLFWwindow *win) {
//    cout << __PRETTY_FUNCTION__ << endl;
    glClearColor(0, 0.7, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    /* place the camera using the camera coordinate frame */
    glMultMatrixf (glm::value_ptr(camera_cf));

    const float& S = 1;
    /* draw the axes */
    glBegin(GL_LINES);
    glColor3ub (255, 0, 0);
    glVertex2i (0, 0);
    glVertex2f (S * 1.1, 0);
    glColor3ub (0, 255, 0);
    glVertex2i (0, 0);
    glVertex2f (0, S * 1.1);
    glColor3ub (0, 0, 255);
    glVertex2i (0, 0);
    glVertex3f (0, 0, S * 1.1);
    glEnd();

    /* The following two groups of GL_LINE_LOOP and GL_LINES draw the square block
     * whose 4 vertices make the tetrahedron */

    //enable_contents();
    glPushMatrix();
    glTranslatef(1.3, 0, 0);
    pipe.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3, 0, 0);
    glRotatef(180, 0, 1, 0);
    pipe.render();
    glPopMatrix();
    /* must swap buffer at the end of render function */
    glfwSwapBuffers(win);
}



/* action: GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT */
void key_handler (GLFWwindow *win, int key, int scan_code, int action, int mods)
{
    const float FIRST_VIEW = 0.5f;
    const float SECOND_VIEW = 1.5f;
    glm::vec3 N;
    float theta;
    glm::quat q;
    static glm::mat4 current_cam;
    glm::vec3 this_vec;
    cout << __FUNCTION__ << endl;
    if (action != GLFW_PRESS) return;
    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_D: /* Uppercase 'D' */
                /* pre mult: trans  Z-ax of the world */
//                hex1_cf = glm::translate(glm::vec3{0, +0.5f, 0}) * hex1_cf;
                break;
        }
    }
    else {
        switch (key) {
            case GLFW_KEY_W:
                glPolygonMode(GL_FRONT, GL_LINE);
                break;
            case GLFW_KEY_C:
                camera_cf = camera_cf * glm::translate(glm::vec3{-5,-5,5});
                break;
            case GLFW_KEY_LEFT:

                break;
            case GLFW_KEY_RIGHT:

                break;
            case GLFW_KEY_UP:

                break;
            case GLFW_KEY_DOWN:


                break;
            case GLFW_KEY_L:

                break;
            case GLFW_KEY_D: /* lowercase 'd' */
                /* pre mult: trans  Z-ax of the world */
//                hex1_cf = glm::translate(glm::vec3{0, -0.5f, 0}) * hex1_cf;
                break;
            case GLFW_KEY_MINUS:
                /* post mult: rotate around Z-ax of the hex nut */
//                hex1_cf = hex1_cf * glm::rotate(1.0f, glm::vec3{0, 0, 1});
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(win, true);
                break;
            case GLFW_KEY_1:
                this_vec = glm::normalize(glm::vec3{-1, 1, 1});
                N = glm::cross(glm::vec3{-0.5, 0, 0}, this_vec);
                theta = glm::angle(glm::vec3{0, 0, 0}, this_vec);
                camera_cf = current_cam * glm::toMat4(glm::normalize(glm::quat{cos(theta / 2), sin(theta / 2) * N}));

                win_refresh(win);
                break;
            case GLFW_KEY_2:
                this_vec = glm::normalize(glm::vec3{1, 1, 1});
                N = glm::cross(glm::vec3{0.5, 0, 0}, this_vec);
                theta = glm::angle(glm::vec3{0, 0, 0}, this_vec);
                camera_cf = current_cam * glm::toMat4(glm::normalize(glm::quat{cos(theta / 2), sin(theta / 2) * N}));
                //camera_cf =  hex1_cf;
                win_refresh(win);
                break;
            case GLFW_KEY_3:
                this_vec = glm::normalize(glm::vec3{1, 1, 1});
                N = glm::cross(glm::vec3{0, 0, 0.5}, this_vec);
                theta = glm::angle(glm::vec3{0, 0, 0}, this_vec);
                camera_cf = current_cam * glm::toMat4(glm::normalize(glm::quat{cos(theta / 2), sin(theta / 2) * N}));

                win_refresh(win);
                break;
            case GLFW_KEY_4:
                this_vec = glm::normalize(glm::vec3{1, -1, 1});
                N = glm::cross(glm::vec3{0, -0.5, 0}, this_vec);
                theta = glm::angle(glm::vec3{0, 0, 0}, this_vec);
                camera_cf = current_cam * glm::toMat4(glm::normalize(glm::quat{cos(theta / 2), sin(theta / 2) * N}));

                win_refresh(win);
                break;
            case GLFW_KEY_5:
            case GLFW_KEY_6:
                /* rebuild the model at different level of detail */
                break;
        }
    }
    win_refresh(win);
}

/*
    The virtual trackball technique implemented here is based on:
    https://www.opengl.org/wiki/Object_Mouse_Trackball
*/
void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    static glm::vec3 first_click;
    static glm::mat4 current_cam;
    static bool is_tracking = false;

    glm::vec3 this_vec;
    if (state == GLFW_PRESS) {
        /* TODO: use glUnproject? */
        float x = (xpos - screen_ctr_x);
        float y = -(ypos - screen_ctr_y);
        float hypot_square = x * x + y * y;
        float z;

        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
        if (2 * hypot_square < arc_ball_rad_square)
            z = sqrt(arc_ball_rad_square - hypot_square);
        else
            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
        if (!is_tracking) {
            /* store the mouse position when the button was pressed for the first time */
            first_click = glm::normalize(glm::vec3{x, y, z});
            current_cam = camera_cf;
            is_tracking = true;
        }
        else {
            /* compute the rotation w.r.t the initial click */
            this_vec = glm::normalize(glm::vec3{x, y, z});
            /* determine axis of rotation */
            glm::vec3 N = glm::cross(first_click, this_vec);

            /* determine the angle of rotation */
            float theta = glm::angle(first_click, this_vec);

            /* create a quaternion of the rotation */
            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
            /* apply the rotation w.r.t to the current camera CF */
            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
        }
        win_refresh(win);
    }
    else {
        is_tracking = false;
    }
}

void scroll_handler (GLFWwindow *win, double xscroll, double yscroll) {
    /* translate along the camera Z-axis */
    glm::mat4 z_translate = glm::translate((float)yscroll * glm::vec3{0, 0, 1});
    camera_cf =  z_translate * camera_cf;
    win_refresh(win);

}

void init_gl() {
    glEnable (GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_CULL_FACE);
    glLineWidth(3.0);

    /* place the camera at Z=+5 (notice that the sign is OPPOSITE!) */
    camera_cf *= glm::translate(glm::vec3{0, 0, -5});
}

void make_model() {
    pipe.build(0.5, 3, 1, 0.6, 0.6, 0.6, 0.02);
    base.build(3.0/1.414, 0, 3.0/1.414, 0, 0, 4, 0.6, 0.6, 0.6, 0.02);
}

int main() {
    cout << "Hello" << endl;

    if(!glfwInit()) {
        cerr << "Can't initialize GLFW" << endl;
        glfwTerminate();
        exit (EXIT_FAILURE);
    }

    glfwSetErrorCallback(err_function);
    GLFWwindow * win;
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    win = glfwCreateWindow(100, 50, "Test", NULL, NULL);
    if (!win) {
        cerr << "Can't create window" << endl;
        exit (EXIT_FAILURE);
    }

    glfwSetWindowRefreshCallback(win, win_refresh);
    /* On Mac with Retina display there is a discrepancy between units measured in
     * "screen coordinates" and "pixels" */
    // glfwSetWindowSizeCallback(win, win_resize);  /* use this for non-retina displays */
    glfwSetFramebufferSizeCallback(win, win_resize); /* use this for retina displays */
    glfwSetKeyCallback(win, key_handler);
    glfwSetCursorPosCallback(win, cursor_handler);
    glfwSetScrollCallback(win, scroll_handler);
    glfwMakeContextCurrent(win);

    /* glewInit must be invoked AFTER glfwMakeContextCurrent() */
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n", glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    /* GL functions can be called, only AFTER the window is created */
    const GLubyte *version = glGetString (GL_VERSION);
    printf ("GL Version is %s\n", version);


    glfwSetWindowSize(win, 450, 300);
    glfwSwapInterval(1);
    init_gl();
    make_model();

    int ev_num = 0;
    win_refresh(win);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}