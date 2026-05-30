#include "convexHull.h"
#include <iostream>
#include <unordered_set>
#include <cmath>
#include <iostream>

std::function<void()> update;
int partitionStart = -1;
int partitionEnd = -1;
int pointP = -1;
std::vector<int> tempHull = {};

// Hull 2d
std::vector<int> convexHull(std::vector<int>& points) {
    if (points.size() <= 3) return points;

    std::vector<int> hull = {};
    std::vector<int> C1 = {};
    std::vector<int> C2 = {};

    int min = points[0];
    int max = points[points.size() - 1];

    for (auto i: points) {
        if (AllPoints[i].x < AllPoints[min].x) min = i;
        if (AllPoints[i].x > AllPoints[max].x) max = i;
    }

    for (auto i: points) {
        if (i == min || i == max) continue;

        Vector ab = {AllPoints[max].x - AllPoints[min].x, AllPoints[max].y - AllPoints[min].y};
        Vector ap = {AllPoints[i].x - AllPoints[min].x, AllPoints[i].y - AllPoints[min].y};

        float prodV = prodVetorial2d(ab, ap);

        if (prodV > 0) C1.push_back(i);
        else if (prodV < 0) C2.push_back(i);
    }

    auto esq  = auxHull(C1, min, max);
    auto dir = auxHull(C2, max, min);

    hull.insert(hull.end(), esq.begin(), esq.end());
    hull.insert(hull.end(), dir.begin(), dir.end());

    return hull;
}

std::vector<int> auxHull(std::vector<int>& points, int a, int b) {
    if (points.empty()) return {a}; // se for a e b vai ficar com vertices repetidos

    std::vector<int> hull = {};

    int p = 0;
    float maxDist = -1.0f;

    for (auto i : points) {
        Vector ab = {AllPoints[b].x - AllPoints[a].x, AllPoints[b].y - AllPoints[a].y};
        Vector ap = {AllPoints[i].x - AllPoints[a].x, AllPoints[i].y - AllPoints[a].y};

        float dist = prodVetorial2d(ab, ap);

        if (dist > maxDist) {
            maxDist = dist;
            p = i;
        }
    }

    std::vector<int> C1;
    std::vector<int> C2;

    for (auto i : points) {
        if (i == p) continue;

        // esquerda de ap
        Vector ap = { AllPoints[p].x - AllPoints[a].x, AllPoints[p].y - AllPoints[a].y};
        Vector aq = {AllPoints[i].x - AllPoints[a].x, AllPoints[i].y - AllPoints[a].y};

        if (prodVetorial2d(ap, aq) > 0) {
            C1.push_back(i);
            continue;
        }

        // esquerda de pb
        Vector pb = {AllPoints[b].x - AllPoints[p].x, AllPoints[b].y - AllPoints[p].y};
        Vector pq = {AllPoints[i].x - AllPoints[p].x, AllPoints[i].y - AllPoints[p].y};

        if (prodVetorial2d(pb, pq) > 0) {
            C2.push_back(i);
        }
    }

    auto esq = auxHull(C1, a, p);
    auto dir = auxHull(C2, p, b);

    hull.insert(hull.end(), esq.begin(), esq.end());
    hull.insert(hull.end(), dir.begin(), dir.end());

    return hull;
}



// Hull 3d
std::vector<Face> convexHull3d(std::vector<int>& points) {
    if (points.size() < 4) return {};

    std::vector<Face> hull = getFirstTetrahedron(points);

    bool end = false;
    while (!end) {
        end = true;

        Face f = hull[0];
        for (Face& f : hull) {
            if (f.deleted) continue;

            int newP = -1;
            float newMaxDist = -1.0f;

            for (int i : points) {
                if (i == f.a || i == f.b || i == f.c) continue;
                Vector ab = {AllPoints[f.b].x - AllPoints[f.a].x, AllPoints[f.b].y - AllPoints[f.a].y, AllPoints[f.b].z - AllPoints[f.a].z};
                Vector ac = {AllPoints[f.c].x - AllPoints[f.a].x, AllPoints[f.c].y - AllPoints[f.a].y, AllPoints[f.c].z - AllPoints[f.a].z};

                Vector normal = prodVetorial(ab, ac);
                Vector pointVector = {AllPoints[i].x - AllPoints[f.a].x, AllPoints[i].y - AllPoints[f.a].y, AllPoints[i].z - AllPoints[f.a].z};

                float dist = prodEscalar(normal, pointVector);

                if (dist <= 0) continue;

                if (dist > newMaxDist) {
                    newMaxDist = dist;
                    newP = i;
                }
            }

            if (newP == -1) continue;
            end = false; // se nenhuma face achar nenhum ponto o algoritmo nunca chega aqui e para por causa da condicao

            std::vector<int> visibleFaces = getVisibleFaces(hull, newP);
            for (int i : visibleFaces) hull[i].deleted = true;

            std::vector<Edge> horizon = findHorizon(hull, visibleFaces);

            for (Edge e : horizon) {
                if (e.a == e.b || e.a == newP || e.b == newP) continue;

                Face newFace = {e.a, e.b, newP, false};

                if (faceFind(hull, newFace.a, newFace.b, newFace.c)) continue;
                hull.push_back(newFace);
            }

            std::vector<Face> newHull;
            for (Face& face : hull) {
                if (!face.deleted) newHull.push_back(face);
            }

            hull = newHull;

            orientFace(hull);

            break;
        }
    }

    return hull;
}

std::vector<Face> getFirstTetrahedron(std::vector<int>& points) {
    std::vector<Face> hull = {};

    // encontrando o primeiro tetraedro
    int min = points[0];
    int max = points[points.size() - 1];

    for (auto i: points) {
        if (AllPoints[i].x < AllPoints[min].x) min = i;
        if (AllPoints[i].x > AllPoints[max].x) max = i;
    }

    // pontos mais distante da reta
    int p = points[0];
    float maxDist = -1.0f;

    for (auto i : points) {
        Vector ab = {AllPoints[max].x - AllPoints[min].x, AllPoints[max].y - AllPoints[min].y, AllPoints[max].z - AllPoints[min].z};
        Vector ap = {AllPoints[i].x - AllPoints[min].x, AllPoints[i].y - AllPoints[min].y, AllPoints[i].z - AllPoints[min].z};

        float dist = norma(prodVetorial(ab, ap));

        if (dist > maxDist) {
            maxDist = dist;
            p = i;
        }
    }

    // pontos mais distante do plano
    Vector ab = {AllPoints[max].x - AllPoints[min].x, AllPoints[max].y - AllPoints[min].y, AllPoints[max].z - AllPoints[min].z};
    Vector ap = {AllPoints[p].x - AllPoints[min].x, AllPoints[p].y - AllPoints[min].y, AllPoints[p].z - AllPoints[min].z};

    Vector normal = prodVetorial(ab, ap);

    int q = points[0];
    maxDist = -1.0f;

    for (auto i: points) {
        if (i == min || i == max || i == p) continue;

        Vector pointVector = {AllPoints[i].x - AllPoints[min].x, AllPoints[i].y - AllPoints[min].y, AllPoints[i].z - AllPoints[min].z};

        float dist = fabs(prodEscalar(normal, pointVector));

        if (dist > maxDist) {
            maxDist = dist;
            q = i;
        }
    }

    Face f1 = {min, max, p};
    Face f2 = {min, q, max};
    Face f3 = {min, p, q};
    Face f4 = {max, q, p};

    hull.push_back(f1);
    hull.push_back(f2);
    hull.push_back(f3);
    hull.push_back(f4);

    orientFace(hull);

    return hull;
}

void orientFace(std::vector<Face>& faces) {
    Point centroide = {0,0,0};
    int count = 0;

    for (Face& f : faces) {
        centroide.x += AllPoints[f.a].x;
        centroide.y += AllPoints[f.a].y;
        centroide.z += AllPoints[f.a].z;

        centroide.x += AllPoints[f.b].x;
        centroide.y += AllPoints[f.b].y;
        centroide.z += AllPoints[f.b].z;

        centroide.x += AllPoints[f.c].x;
        centroide.y += AllPoints[f.c].y;
        centroide.z += AllPoints[f.c].z;

        count += 3;
    }

    centroide.x = centroide.x / count;
    centroide.y = centroide.y / count;
    centroide.z = centroide.z / count;

    for (Face& f : faces) {
        Vector ab = {AllPoints[f.b].x - AllPoints[f.a].x, AllPoints[f.b].y - AllPoints[f.a].y, AllPoints[f.b].z - AllPoints[f.a].z};
        Vector ac = {AllPoints[f.c].x - AllPoints[f.a].x, AllPoints[f.c].y - AllPoints[f.a].y, AllPoints[f.c].z - AllPoints[f.a].z};

        Vector normal = prodVetorial(ab, ac);

        Vector centroideVetor = {centroide.x - AllPoints[f.a].x, centroide.y - AllPoints[f.a].y, centroide.z - AllPoints[f.a].z};

        float side = prodEscalar(normal, centroideVetor);

        if (side > 0) std::swap(f.b, f.c);
    }
}

bool edgeFind(Face& f, Edge e) {
    return (f.a == e.a || f.b == e.a || f.c == e.a) && (f.a == e.b || f.b == e.b || f.c == e.b);
}

bool faceFind(std::vector<Face>& faces, int a, int b, int c) {
    for (const Face& f : faces) {
        if ((a == f.a || a == f.b || a == f.c) && (b == f.a || b == f.b || b == f.c) && (c == f.a || c == f.b || c == f.c)) return true;
    }

    return false;
}

std::vector<Edge> findHorizon(std::vector<Face>& hull, std::vector<int>& visibleFaces) {
    std::vector<Edge> horizon;

    for (int i : visibleFaces) {
        Face& f = hull[i];
        std::vector<Edge> edges = {{f.a, f.b}, {f.b, f.c}, {f.c, f.a}};

        for (Edge e : edges) {
            int visibleCount = 0;

            for (int j : visibleFaces) {
                if (edgeFind(hull[j], e)) visibleCount++;
            }

            if (visibleCount == 1) horizon.push_back(e);
        }
    }

    return horizon;
}

std::vector<int> getVisibleFaces(std::vector<Face>& hull, int p) {
    std::vector<int> visibleFaces = {};

    for (int i = 0; i < hull.size(); i++) {
        if (hull[i].deleted) continue;
        Vector ab = {AllPoints[hull[i].b].x - AllPoints[hull[i].a].x, AllPoints[hull[i].b].y - AllPoints[hull[i].a].y, AllPoints[hull[i].b].z - AllPoints[hull[i].a].z};
        Vector ac = {AllPoints[hull[i].c].x - AllPoints[hull[i].a].x, AllPoints[hull[i].c].y - AllPoints[hull[i].a].y, AllPoints[hull[i].c].z - AllPoints[hull[i].a].z};

        Vector normal = prodVetorial(ab, ac);

        Vector ap = {AllPoints[p].x - AllPoints[hull[i].a].x, AllPoints[p].y - AllPoints[hull[i].a].y, AllPoints[p].z - AllPoints[hull[i].a].z};

        float side = prodEscalar(normal, ap);

        if (side > 0) visibleFaces.push_back(i);
    }

    return visibleFaces;
}




// Parte de Render
void drawNormal(Face face) {
    Point a = AllPoints[face.a];
    Point b = AllPoints[face.b];
    Point c = AllPoints[face.c];

    Vector ab = {b.x - a.x,b.y - a.y,b.z - a.z};

    Vector ac = {c.x - a.x,c.y - a.y,c.z - a.z};

    Vector n = prodVetorial(ab, ac);

    Point center = {(a.x + b.x + c.x) / 3.0f,(a.y + b.y + c.y) / 3.0f,(a.z + b.z + c.z) / 3.0f};

    Point end = {center.x + n.x * 0.001f, center.y + n.y * 0.001f, center.z + n.z * 0.001f};

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);

    glVertex3f(center.x, center.y, center.z);
    glVertex3f(end.x, end.y, end.z);

    glEnd();
    glColor3f(1.0f, 0.0f, 1.0f);
}

void render(const Object &polygon, GLFWwindow* window) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex2f(SCREEN_MIN, 0);
    glVertex2f(SCREEN_MAX, 0);
    glVertex2f(0, SCREEN_MIN);
    glVertex2f(0, SCREEN_MAX);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.8f, 1.0f);
    for (const auto& p : polygon.convex_hull) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for(const auto& p : polygon.convex_hull) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(8);

    if (partitionStart >= 0) {
        glBegin(GL_POINTS);
        glVertex2f(AllPoints[partitionStart].x, AllPoints[partitionStart].y);
        glEnd();
    }

    if (partitionEnd >= 0) {
        glBegin(GL_POINTS);
        glVertex2f(AllPoints[partitionEnd].x, AllPoints[partitionEnd].y);
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 1.0f);

    if (partitionStart >= 0 && partitionEnd >= 0) {
        glBegin(GL_LINES);
        glVertex2f(AllPoints[partitionStart].x, AllPoints[partitionStart].y);
        glVertex2f(AllPoints[partitionEnd].x, AllPoints[partitionEnd].y);
        glEnd();
    }

    if (pointP >= 0) {
        Point A = AllPoints[partitionStart];
        Point B = AllPoints[partitionEnd];
        Point P = AllPoints[pointP];

        float abx = B.x - A.x;
        float aby = B.y - A.y;

        float apx = P.x - A.x;
        float apy = P.y - A.y;

        float dot = apx * abx + apy * aby;
        float ab2 = abx * abx + aby * aby;

        float t = dot / ab2;

        Point Q = {A.x + abx * t, A.y + aby * t};

        glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_LINES);

        glVertex2f(P.x, P.y);
        glVertex2f(Q.x, Q.y);

        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(AllPoints[pointP].x, AllPoints[pointP].y);
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
    for(const auto& p : tempHull) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(4);

    glBegin(GL_POINTS);
    for(const auto& p : polygon.points) {
        glVertex2f(AllPoints[p].x, AllPoints[p].y);
    }
    glEnd();

    glfwSwapBuffers(window);
}

std::vector<int> renderConvexHull(std::vector<int>& points) {
    if (points.size() <= 3) return points;

    std::vector<int> hull = {};
    std::vector<int> C1 = {};
    std::vector<int> C2 = {};

    int min = 0;
    int max = 0;

    for (auto i: points) {
        if (AllPoints[i].x < AllPoints[min].x) min = i;
        if (AllPoints[i].x > AllPoints[max].x) max = i;
    }

    partitionStart = min;
    partitionEnd = max;

    update();

    for (auto i: points) {
        if (i == min || i == max) continue;

        pointP = i;
        Vector ab = {AllPoints[max].x - AllPoints[min].x, AllPoints[max].y - AllPoints[min].y};
        Vector ap = {AllPoints[i].x - AllPoints[min].x, AllPoints[i].y - AllPoints[min].y};

        float prodV = prodVetorial2d(ab, ap);

        if (prodV > 0) C1.push_back(i);
        else if (prodV < 0) C2.push_back(i);
    }

    auto esq  = renderAuxHull(C1, min, max);
    auto dir = renderAuxHull(C2, max, min);

    hull.insert(hull.end(), esq.begin(), esq.end());
    hull.insert(hull.end(), dir.begin(), dir.end());

    return hull;
}

std::vector<int> renderAuxHull(std::vector<int>& points, int a, int b) {
    if (points.empty()) {
        tempHull.push_back(a);
        return {a};
    }

    std::vector<int> hull = {};

    partitionStart = a;
    partitionEnd = b;
    pointP = -1;

    update();

    int p = 0;
    float maxDist = -1.0f;

    for (auto i : points) {
        pointP = i;
        update();

        Vector ab = {AllPoints[b].x - AllPoints[a].x, AllPoints[b].y - AllPoints[a].y};
        Vector ap = {AllPoints[i].x - AllPoints[a].x, AllPoints[i].y - AllPoints[a].y};

        float dist = prodVetorial2d(ab, ap);

        if (dist > maxDist) {
            maxDist = dist;
            p = i;
        }
    }

    tempHull.push_back(p);

    std::vector<int> C1;
    std::vector<int> C2;

    for (auto i : points) {
        if (i == p) continue;

        Vector ap = { AllPoints[p].x - AllPoints[a].x, AllPoints[p].y - AllPoints[a].y};
        Vector aq = {AllPoints[i].x - AllPoints[a].x, AllPoints[i].y - AllPoints[a].y};

        if (prodVetorial2d(ap, aq) > 0)
        {
            C1.push_back(i);
            continue;
        }

        Vector pb = {AllPoints[b].x - AllPoints[p].x, AllPoints[b].y - AllPoints[p].y};
        Vector pq = {AllPoints[i].x - AllPoints[p].x, AllPoints[i].y - AllPoints[p].y};

        if (prodVetorial2d(pb, pq) > 0)
        {
            C2.push_back(i);
        }
    }

    auto esq = renderAuxHull(C1, a, p);
    auto dir = renderAuxHull(C2, p, b);

    hull.insert(hull.end(), esq.begin(), esq.end());
    hull.insert(hull.end(), dir.begin(), dir.end());

    return hull;
}