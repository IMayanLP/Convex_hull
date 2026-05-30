#include <iostream>
#include "utils/convexHull.h"

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(SCREEN_SIZE, SCREEN_SIZE, "Trabalho 1 - Fecho Convexo", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(SCREEN_MIN, SCREEN_MAX, SCREEN_MIN, SCREEN_MAX, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::vector<Object> theme;
    theme = carregarOBJ("../assets/theme.obj");

    for (auto object: theme) {
        std::cout << object.name << std::endl;
        for (auto p : object.points) {
            std::cout << AllPoints[p].id << " ";
        }
        std::cout << std::endl;
    }

    for (auto& object : theme) {
        object.convex_hull = convexHull(object.points);
    }

    for (auto object: theme) {
        std::cout << "Fecho convexo do grupo: " << object.name << std::endl;
        for (auto p : object.convex_hull) {
            std::cout << AllPoints[p].id << " ";
        }
        std::cout << std::endl;
    }

    float angle = 0.0f;

    exportarOBJ("../assets/output.obj", theme);

    bool showInternalPoints = true;
    bool spacePressedLastFrame = false;

    while (!glfwWindowShouldClose(window)) {
        angle += 0.5f;
        if (angle >= 360.0f) angle -= 360.0f;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (!spacePressedLastFrame) {
                showInternalPoints = !showInternalPoints;
                spacePressedLastFrame = true;
            }
        } else spacePressedLastFrame = false;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.0f, 0.0f, 0.0f);

        glBegin(GL_LINES);
        glVertex2f(SCREEN_MIN, 0);
        glVertex2f(SCREEN_MAX, 0);
        glVertex2f(0, SCREEN_MIN);
        glVertex2f(0, SCREEN_MAX);
        glEnd();

        glPointSize(4);
        glColor3f(0.0f, 0.0f, 1.0f);
        for (auto object: theme) {
            if (object.name == "Pa") {
                for (int i = 0; i < 4; i++) {
                    glPushMatrix();
                    glRotatef(angle + (i * 90.0f), 0.0f, 0.0f, 1.0f);
                    printPolygon(object.convex_hull, object.color);
                    if (showInternalPoints) printPoints(object.points);
                    glPopMatrix();
                }
            } else if (object.name == "PaCentro") {
                glPushMatrix();
                glRotatef(angle, 0.0f, 0.0f, 1.0f);
                printPolygon(object.convex_hull, object.color);
                if (showInternalPoints) printPoints(object.points);
                glPopMatrix();
            } else {
                printPolygon(object.convex_hull, object.color);
                if (showInternalPoints) printPoints(object.points);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
