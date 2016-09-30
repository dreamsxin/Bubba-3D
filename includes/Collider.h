#pragma once

#include "Scene.h"
#include "linmath/float4x4.h"

class Octree;

bool octreeOctreeIntersection(Octree *object1Octree,
                              chag::float4x4 *object1ModelMatrix,
                              Octree *object2Octree,
                              chag::float4x4 *object2ModelMatrix);

bool AabbAabbintersection(AABB *aabb1, AABB *aabb2);

bool sphereIntersection(Sphere sphere1,Sphere sphere2);

AABB multiplyAABBWithModelMatrix(AABB *aabb, chag::float4x4 modelMatrix);

bool triangleTriangleIntersection(Triangle *t1, Triangle *t2);

bool isTrianglesIntersecting(Octree *object1Octree,
                             chag::float4x4 *object1ModelMatrix,
                             Octree *object2Octree,
                             chag::float4x4 *object2ModelMatrix);


/**
 * Class responsible for dynamic collision tests in a scene.
 * Every colliding pair in the scene will at collisions have
 * a collision event called.
 */
class Collider
{
public:
    /**
     * Calls collision events on all colliding scene objects.
     */
    virtual void updateCollision(Scene *scene) = 0;

private:

};