#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SCREEN_SIZE 800.0
#define SCREEN_MIN -400.0
#define SCREEN_MAX 400.0

struct Color {
    float r, g, b;
};

struct Point {
    float x, y, z;
    int id;
    Color color = {1, 0, 0};
};

struct Edge {
    int a, b;
};

struct Face {
    int a, b, c;
    bool deleted = false;
};

struct Vector {
    float x, y, z;
};

struct Object {
    std::string name;
    Color color;
    std::vector<int> points;
    std::vector<int> convex_hull;
    std::vector<Face> convex_hull_3d;
};

extern std::vector<Point> AllPoints;

std::vector<int> gerarPontosAleatorios(int, bool);

float prodVetorial2d(Vector, Vector);
Vector prodVetorial(Vector, Vector);

float norma(Vector);
float prodEscalar(Vector, Vector);

bool ccw(Vector, Vector);
bool cw(Vector, Vector);

std::vector<Object> carregarOBJ(const std::string&);
void exportarOBJ(const std::string&, const std::vector<Object>&);
void printPoints(std::vector<int>&);
void printPolygon(std::vector<int>&, Color);