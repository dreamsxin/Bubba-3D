//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include <Collider.h>
#include <timer.h>
#include <Logger.h>
#include <sstream>
#include <SFML/System/Clock.hpp>
#include "BFBroadPhase.h"

BFBroadPhase::BFBroadPhase() {

}

CollisionPairList BFBroadPhase::computeCollisionPairs(Scene *scene) {
    std::vector<GameObject*> sceneObjects = scene->getGameObjects();
    CollisionPairList collisionPairs;

    for (auto i = sceneObjects.begin(); i != sceneObjects.end(); i++) {
        for (auto j = i + 1; j != sceneObjects.end(); j++) {
            GameObject* gameObject1 = *i;
            GameObject* gameObject2 = *j;

            if(isPossiblyColliding(gameObject1,gameObject2)) {
                collisionPairs.push_back(std::pair<GameObject *, GameObject *>(gameObject1, gameObject2));
            }

        }
    }
    return collisionPairs;
}


bool BFBroadPhase::isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2) {
    bool noDynamic = !gameObject1->isDynamicObject() && !gameObject2->isDynamicObject();
    bool neverCollides = !gameObject1->collidesWith(gameObject2->getIdentifier())
                         && !gameObject2->collidesWith(gameObject1->getIdentifier());
    if(noDynamic || neverCollides || gameObject1->isDirty() || gameObject2->isDirty() || gameObject1 == gameObject2) {
        return false;
    }
    bool sphereInt = sphereIntersection(gameObject1->getTransformedSphere(),gameObject2->getTransformedSphere());
    if(sphereInt) {

        AABB aabb1 = gameObject1->getTransformedAABB();
        AABB aabb2 = gameObject2->getTransformedAABB();

        bool aabb = AabbAabbintersection(&aabb1, &aabb2);
        return aabb;
    }

    return false;
}