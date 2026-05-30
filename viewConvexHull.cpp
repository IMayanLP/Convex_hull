#include <iostream>
#include "utils/convexHull.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(SCREEN_SIZE, SCREEN_SIZE, "Trabalho 1 - Fecho Convexo", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(SCREEN_MIN, SCREEN_MAX, SCREEN_MIN, SCREEN_MAX, -1.0, 1.0);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Object polygon;

    update = [&]() {
        render(polygon, window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    };

    polygon.points = gerarPontosAleatorios(25, false);
    polygon.convex_hull = renderConvexHull(polygon.points);

    pointP = -1;
    partitionStart = -1;
    partitionEnd = -1;

    update();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
