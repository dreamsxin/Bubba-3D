/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */

#include "Triangle.h"
#include "Octree.h"

#define DEFAULT_ORIGIN chag::make_vector(0.0f, 0.0f, 0.0f)
#define DEFAULT_HALFVECTOR chag::make_vector(1.0f, 1.0f, 1.0f)

Octree::Octree() {
    clearChildren();
    origin = DEFAULT_ORIGIN;
    halfVector = DEFAULT_HALFVECTOR;
    setupAABB(DEFAULT_ORIGIN, DEFAULT_HALFVECTOR);
}

Octree::~Octree() {

}

Octree::Octree(chag::float3 origin, chag::float3 halfVector)
        : origin(origin), halfVector(halfVector)
{
    depth = 0;
    clearChildren();
    setupAABB(origin, halfVector);
}

Octree::Octree(chag::float3 origin, chag::float3 halfVector, int depth)
        : origin(origin), halfVector(halfVector), depth(depth)
{
    clearChildren();
    setupAABB(origin, halfVector);
}

void Octree::clearChildren() {
    for (int i = 0; i < 8; i++) {
        children[i] = NULL;
    }
}

void Octree::setupAABB(chag::float3 origin, chag::float3 halfVector) {
    chag::float3 p1 = origin + halfVector;
    chag::float3 p2 = origin - halfVector;

    aabb.maxV = combineTwoPointsByComparator(p1,p2, [](float point1, float point2)
    {
       return point1 > point2;
    });
    aabb.minV = combineTwoPointsByComparator(p1,p2, [](float point1, float point2)
    {
       return point1 < point2;
    });
}

chag::float3 Octree::combineTwoPointsByComparator(
    chag::float3 p1, chag::float3 p2,
    std::function<bool (float ,float)> comparator)
{
    chag::float3 maxPoint;

    maxPoint.x = comparator(p1.x, p2.x) ? p1.x : p2.x;
    maxPoint.y = comparator(p1.y, p2.y) ? p1.y : p2.y;
    maxPoint.z = comparator(p1.z, p2.z) ? p1.z : p2.z;

    return maxPoint;
}

std::vector<Triangle*> *Octree::getTriangles() {
    return &ts;
}


void Octree::insertAll(std::vector<Triangle *> &triangles) {
    for (unsigned int i = 0; i < triangles.size(); i++) {
        insertTriangle(triangles[i]);
    }
}

void Octree::insertTriangle(Triangle *t) {
    BoundingBox *boundingBox = t->getBoundingBox(); //TODO USE Triangle/AABB TEST
    std::set<int> octs;
    for (int i = 0; i < 8; i++) {
        octs.insert(getOctantContainingPoint(boundingBox->points[i]));
    }

    if(octs.size() > 1 || depth == MAX_DEPTH) {
        ts.push_back(t);
    } else {
        if(!hasChildren()) {
            createChildren();
        }

        for (std::set<int>::iterator it = octs.begin(); it != octs.end(); it++) {
            children[*it]->insertTriangle(t);
        }
    }
}

void Octree::createChildren() {
    for (int i = 0; i < 8; ++i) {
        chag::float3 newOrigin = origin;
        newOrigin.x += halfVector.x * (i & 4 ? .5f : -.5f);
        newOrigin.y += halfVector.y * (i & 2 ? .5f : -.5f);
        newOrigin.z += halfVector.z * (i & 1 ? .5f : -.5f);
        children[i] = new Octree(newOrigin, halfVector * .5f, depth + 1);
    }
}

bool Octree::hasChildren() {
    return *children != NULL;
}

void Octree::getChildren(std::vector<Octree*> *octs) {
    if (hasChildren()) {
        for (int i = 0; i < 8; i++) {
            octs->push_back(children[i]);
        }
    }
}


int Octree::getOctantContainingPoint(const chag::float3 &point) {
    int oct = 0;
    if (point.x >= origin.x) oct |= 4;
    if (point.y >= origin.y) oct |= 2;
    if (point.z >= origin.z) oct |= 1;
    return oct;
}

int Octree::getTriangleCount() {
    return ts.size();
}

AABB* Octree::getAABB() {
    return &aabb;
}

int Octree::getTriangleCountRecursively() {
    int c = getTriangleCount();

    if (hasChildren()) {
        for (int i = 0; i < 8; i++) {
            c += children[i]->getTriangleCountRecursively();
        }
    }

    return c;
}

int Octree::getNumberOfSubTrees() {
    int count = 1;
    if(hasChildren()) {
        for (int i = 0; i < 8; i++) {
            count += children[i]->getNumberOfSubTrees();
        }
    }

    return count;
}

bool testSlab(float rayO, float rayD, float minV, float maxV, float *tNear, float *tFar) {

    if (rayD < 0.0001f) {
        return rayO <= maxV && rayO >= minV;
    }
    else {

        float t1 = (minV - rayO) * rayD;
        float t2 = (maxV - rayO) * rayD;

        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > *tNear) {
            *tNear = t1;
        }
        if (t2 < *tFar) {
            *tFar = t2;
        }

        if (*tNear < *tFar) {
            return false;
        }

        if (*tFar < 0) {
            return false;
        }
    }

    return true;
}


bool Octree::rayCastIntersectsAABB(chag::float3 rayOrigin, chag::float3 rayVector) {
    chag::float3 maxCorner = chag::make_vector(origin.x + halfVector.x, origin.y + halfVector.y, origin.z + halfVector.z);
    chag::float3 minCorner = chag::make_vector(origin.x - halfVector.x, origin.y - halfVector.y, origin.z - halfVector.z);

    float tNear = -FLT_MAX, tFar = FLT_MAX;

    return testSlab(rayOrigin.x, rayVector.x, minCorner.x, maxCorner.x, &tNear, &tFar)
           || (testSlab(rayOrigin.y, rayVector.y, minCorner.y, maxCorner.y, &tNear, &tFar))
           || (testSlab(rayOrigin.z, rayVector.z, minCorner.z, maxCorner.z, &tNear, &tFar));
}

// TODO(Bubbad) Only add triangles if they are actually intersected
void Octree::getTrianglesInsersectedByRayCast(chag::float3 rayOrigin, chag::float3 rayVector, std::vector<Triangle *> *triangleList) {
    if (!rayCastIntersectsAABB(rayOrigin, rayVector)) {
        return;
    }

    putTrianglesToList(triangleList);
    if (hasChildren()) {
        for (int i = 0; i < 8; i++) {
            children[i]->getTrianglesInsersectedByRayCast(rayOrigin, rayVector, triangleList);
        }
    }
}

void Octree::putTrianglesToList(std::vector<Triangle *> *triangleList) {
    for (unsigned int i = 0; i < ts.size(); i++) {
        triangleList->push_back(ts[i]);
    }
}
