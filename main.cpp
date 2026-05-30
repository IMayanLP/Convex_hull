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

    Object polygon;
    polygon.points = gerarPontosAleatorios(1000, false);
    polygon.convex_hull = convexHull(polygon.points);

    std::cout << "Fecho Convexo: ";
    for (auto i: polygon.convex_hull) {
        std::cout << AllPoints[i].id << " ";
    }

    while (!glfwWindowShouldClose(window)) {
        render(polygon, window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
