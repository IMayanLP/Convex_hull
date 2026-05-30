#include "utils.h"
#include <functional>
#include <thread>

extern std::function<void()> update;
extern int partitionStart;
extern int partitionEnd;
extern int pointP;
extern std::vector<int> tempHull;

std::vector<int> convexHull(std::vector<int>&);
std::vector<int> auxHull(std::vector<int>&, int, int);

std::vector<Face> convexHull3d(std::vector<int>&);
bool edgeFind(Face&, Edge);
bool faceFind(std::vector<Face>&, int, int, int);
void orientFace(std::vector<Face>&);
std::vector<Face> getFirstTetrahedron(std::vector<int>&);
std::vector<int> getVisibleFaces(std::vector<Face>&, int);
std::vector<Edge> findHorizon(std::vector<Face>&, std::vector<int>&);

void drawNormal(Face);

void render(const Object&, GLFWwindow*);
std::vector<int> renderConvexHull(std::vector<int>&);
std::vector<int> renderAuxHull(std::vector<int>&, int, int);