#include <iostream>
#include "utils/convexHull.h"

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(SCREEN_SIZE, SCREEN_SIZE, "Trabalho 1 - Fecho Convexo", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSwapInterval(1);

    glViewport(0, 0, SCREEN_SIZE, SCREEN_SIZE);

    glClearColor(0.1f, 0.1f, 0.125f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glShadeModel(GL_FLAT);

    GLfloat lightPos[] = { 10.0f, 3.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(SCREEN_MIN, SCREEN_MAX, SCREEN_MIN, SCREEN_MAX, -5000, 5000);
    glMatrixMode(GL_MODELVIEW);

    Object polygon;
    polygon.points = gerarPontosAleatorios(50, true);
    polygon.convex_hull_3d = convexHull3d(polygon.points);

    for (Face& f : polygon.convex_hull_3d) {
        AllPoints[f.a].color = {0, 1, 0};
        AllPoints[f.b].color = {0, 1, 0};
        AllPoints[f.c].color = {0, 1, 0};
    }

    std::cout << "Fecho convexo 3d: " << std::endl;
    for (auto f : polygon.convex_hull_3d) {
        std::cout << "F: (" << f.a << " " << f.b << " " << f.c << ", [" << f.deleted << "]), " ;
    }

    float rotationX = 0.0f;
    float rotationY = 0.0f;

    bool showSolidTriangles = false;
    bool spacePressedLastFrame = false;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rotationY -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rotationY += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rotationX -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) rotationX += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotationY -= 0.2f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationY += 0.2f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotationX -= 0.2f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) rotationX += 0.2f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (!spacePressedLastFrame) {
                showSolidTriangles = !showSolidTriangles;
                spacePressedLastFrame = true;
            }
        } else spacePressedLastFrame = false;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -25.0f);

        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        glDisable(GL_LIGHTING);

        glLineWidth(1.0f);
        glBegin(GL_LINES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(SCREEN_MIN, 0.0f, 0.0f);
        glVertex3f(SCREEN_MAX, 0.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, SCREEN_MIN, 0.0f);
        glVertex3f(0.0f, SCREEN_MAX, 0.0f);

        glColor3f(0.0f, 0.5f, 1.0f);
        glVertex3f(0.0f, 0.0f, SCREEN_MIN);
        glVertex3f(0.0f, 0.0f, SCREEN_MAX);

        glEnd();

        glPointSize(4.0f);
        glBegin(GL_POINTS);
        for (auto p: polygon.points) {
            glColor3f(AllPoints[p].color.r, AllPoints[p].color.g, AllPoints[p].color.b);
            glVertex3f(AllPoints[p].x, AllPoints[p].y, AllPoints[p].z);
        }
        glEnd();

        glColor3f(1.0f, 0.0f, 1.0f);
        for (auto f: polygon.convex_hull_3d) {
            if (!f.deleted) {
                glBegin(GL_LINE_LOOP);
                glVertex3f(AllPoints[f.a].x, AllPoints[f.a].y, AllPoints[f.a].z);
                glVertex3f(AllPoints[f.b].x, AllPoints[f.b].y, AllPoints[f.b].z);
                glVertex3f(AllPoints[f.c].x, AllPoints[f.c].y, AllPoints[f.c].z);
                glEnd();
                drawNormal(f);
            }
        }

        glEnable(GL_LIGHTING);

        if (showSolidTriangles) {
            for (auto f: polygon.convex_hull_3d) {
                if (!f.deleted) {
                    glBegin(GL_TRIANGLES);
                    glVertex3f(AllPoints[f.a].x, AllPoints[f.a].y, AllPoints[f.a].z);
                    glVertex3f(AllPoints[f.b].x, AllPoints[f.b].y, AllPoints[f.b].z);
                    glVertex3f(AllPoints[f.c].x, AllPoints[f.c].y, AllPoints[f.c].z);
                    glEnd();
                    drawNormal(f);
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

