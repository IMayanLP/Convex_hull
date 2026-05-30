#include "utils.h"
#include <random>
#include <fstream>
#include <sstream>
#include <string>

std::mt19937 gen(std::random_device{}());
std::vector<Point> AllPoints;

std::vector<int> gerarPontosAleatorios(int qtd, bool zAxis) {
    std::vector<int> pontos;

    std::uniform_real_distribution distX;
    std::uniform_real_distribution distY;
    std::uniform_real_distribution distZ;

    if (zAxis) {
        distX = std::uniform_real_distribution(SCREEN_MIN / 2, SCREEN_MAX / 2);
        distY = std::uniform_real_distribution(SCREEN_MIN / 2, SCREEN_MAX / 2);
        distZ = std::uniform_real_distribution(SCREEN_MIN / 2, SCREEN_MAX / 2);

    }
    else {
        distX = std::uniform_real_distribution(SCREEN_MIN, SCREEN_MAX);
        distY = std::uniform_real_distribution(SCREEN_MIN, SCREEN_MAX);
        distZ = std::uniform_real_distribution(SCREEN_MIN, SCREEN_MAX);
    }

    for(int i = 0; i < qtd; i++) {
        Point p;
        p.x = distX(gen);
        p.y = distY(gen);
        if (zAxis) p.z = distZ(gen);
        p.id = AllPoints.size();

        AllPoints.push_back(p);
        pontos.push_back(p.id);
    }

    return pontos;
}

float prodVetorial2d(Vector a, Vector b) {
    return (a.x * b.y) - (a.y * b.x);
}

Vector prodVetorial(Vector a, Vector b) {
    return {
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };
}

float norma(Vector a) {
    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

float prodEscalar(Vector a, Vector b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

bool ccw(Vector a, Vector b) {
    return prodVetorial2d(a, b) > 0.0f;
}

bool cw(Vector a, Vector b) {
    return prodVetorial2d(a, b) < 0.0f;
}

std::vector<Object> carregarOBJ(const std::string& caminhoArquivo) {
    std::ifstream file(caminhoArquivo);
    std::vector<Object> objects;

    if (!file.is_open()) return objects;

    std::string linha;
    Object* currentObject = nullptr;

    while (std::getline(file, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        if (linha.rfind("g ", 0) == 0) {
            std::istringstream iss(linha);

            char lixo;
            std::string nome;
            iss >> lixo >> nome;

            Object obj;
            obj.name = nome;
            obj.color = {0.8f, 0.8f, 1.0f};

            objects.push_back(obj);
            currentObject = &objects.back();

            continue;
        }

        if (linha.rfind("v ", 0) == 0) {
            if (currentObject == nullptr)
                continue;

            std::istringstream iss(linha);

            char lixo;
            Point p;
            iss >> lixo >> p.x >> p.y >> p.z;

            int idEncontrado = -1;

            for (const Point& existing : AllPoints) {
                if (existing.x == p.x && existing.y == p.y && existing.z == p.z) {
                    idEncontrado = existing.id;
                    break;
                }
            }

            if (idEncontrado != -1) currentObject->points.push_back(idEncontrado);
            else {
                p.id = AllPoints.size();
                AllPoints.push_back(p);
                currentObject->points.push_back(p.id);
            }
        }

        if (linha.rfind("c ", 0) == 0) {
            if (currentObject == nullptr) continue;

            std::istringstream iss(linha);
            char lixo;
            Color cor;

            if (iss >> lixo >> cor.r >> cor.g >> cor.b) {
                currentObject->color = cor;
            }
        }

    }

    return objects;
}

void exportarOBJ(const std::string& caminhoArquivo, const std::vector<Object>& objects) {
    std::ofstream file(caminhoArquivo);

    if (!file.is_open()) return;

    file << "o Theme\n";

    for (const auto& object : objects) {
        file << "g " << object.name << "\n";

        for (int p : object.points) {
            const Point& temp = AllPoints[p];
            file << "v " << temp.x << " " << temp.y << " " << "0.000000\n";
        }

        file << "f ";

        for (int p : object.convex_hull){
            file << AllPoints[p].id + 1 << " ";
        }

        file << "\n\n";
    }

    file.close();
}

void printPoints(std::vector<int>& points) {
    glBegin(GL_POINTS);
    for (auto p : points) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();
}

void printPolygon(std::vector<int>& points, Color c) {
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POLYGON);
    for (auto p : points) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (auto p : points) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();
}