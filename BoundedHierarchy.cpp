//
//  BoundingBoxHierarchy.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include <limits>
#include <vector>
#include <list>
#include <iostream>

#include "BoundedHierarchy.hpp"


// inspired by https://medium.com/@bromanz/how-to-create-awesome-accelerators-the-surface-area-heuristic-e14b5dec6160

// automatic split axis choice
BoundedHierarchy::BoundedHierarchy(std::vector<ShadableObject*> objects) : root(buildHierarchy(objects, nullptr)) {
    // must set root bounding box after the hierarchy is built since this is normally called by the parent
    root->boundingBox = BoundingBox(root->nodeLeft->boundingBox, root->nodeRight->boundingBox);
}

// manual split axis choice
BoundedNode* BoundedHierarchy::buildHierarchy(std::vector<ShadableObject*> objects, BoundedNode* nodeParent) {
    if(objects.size() < 1) {
        throw std::runtime_error("Object list is empty");
    } else if(objects.size() == 1) {
        // base case
        BoundedNode *leaf = new BoundedNode();
        ShadableObject* object = objects.at(0);
        leaf->object = object;
        leaf->boundingBox = object->boundingBox;
        leaf->nodeParent = nodeParent;
        leaf->nodeLeft = nullptr;
        leaf->nodeRight = nullptr;
        return leaf;
    }
    // recursive case
    
    float splitCostBest[3];
    int splitIndexBest[3];
    BoundingBox boundingBoxLeft[3];
    BoundingBox boundingBoxRight[3];
    std::vector<ShadableObject*> sortedObjects[3];
    
    // iterate through split axes
    for(int splitAxis = 0; splitAxis < 3; splitAxis++) {
        glm::vec3 maskAxis(0);
        if(splitAxis == 0) {
            maskAxis = glm::vec3(1.0, 0.0, 0.0);
        } else if (splitAxis == 1) {
            maskAxis = glm::vec3(0.0, 1.0, 0.0);
        } else if (splitAxis == 2) {
            maskAxis = glm::vec3(0.0, 0.0, 1.0);
        }
        
        // sort objects by position
        sortedObjects[splitAxis].assign(objects.begin(), objects.end());
        
        std::sort(sortedObjects[splitAxis].begin(), sortedObjects[splitAxis].end(), [maskAxis](ShadableObject* a, ShadableObject* b) {
            return glm::dot(maskAxis, a->boundingBox.pointPositive) < glm::dot(maskAxis, b->boundingBox.pointPositive);
        });
        
        std::list<BoundingBox> unionLeft;
        std::list<BoundingBox> unionRight;
        std::list<int> primitiveCountLeft;
        std::list<int> primitiveCountRight;
        
        BoundingBox lastBox;
        int lastPrimitiveCount;
        
        // pre-compute left union and primitive count
        for(int i = 0; i < sortedObjects[splitAxis].size() - 1; i++) {
            if(i == 0) {
                lastBox = sortedObjects[splitAxis].at(i)->boundingBox;
                lastPrimitiveCount = sortedObjects[splitAxis].at(i)->primitiveCount;
            } else {
                lastBox = BoundingBox(sortedObjects[splitAxis].at(i)->boundingBox, lastBox);
                lastPrimitiveCount += sortedObjects[splitAxis].at(i)->primitiveCount;
            }
            unionLeft.push_back(lastBox);
            primitiveCountLeft.push_back(lastPrimitiveCount);
        }
        
        // pre-compute right union and primitive count
        for(int i = sortedObjects[splitAxis].size() - 1; i > 0; i--) {
            if(i == sortedObjects[splitAxis].size() - 1) {
                lastBox = sortedObjects[splitAxis].at(i)->boundingBox;
                lastPrimitiveCount = sortedObjects[splitAxis].at(i)->primitiveCount;
            } else {
                lastBox = BoundingBox(sortedObjects[splitAxis].at(i)->boundingBox, lastBox);
                lastPrimitiveCount += sortedObjects[splitAxis].at(i)->primitiveCount;
            }
            unionRight.push_front(lastBox);
            primitiveCountRight.push_front(lastPrimitiveCount);
        }
        
        // decide which one is optimal
        float splitCost;
        int splitIndex = 0;
        
        std::list<BoundingBox>::iterator itUnionLeft = unionLeft.begin();
        std::list<BoundingBox>::iterator itUnionRight = unionRight.begin();
        std::list<int>::iterator itPrimitiveCountLeft = primitiveCountLeft.begin();
        std::list<int>::iterator itPrimitiveCountRight = primitiveCountRight.begin();
        
        for(; itUnionLeft != unionLeft.end() && itUnionRight != unionRight.end(); ++itUnionLeft, ++itUnionRight, ++itPrimitiveCountLeft, ++itPrimitiveCountRight) {
            splitCost = itUnionLeft->surfaceArea() * *itPrimitiveCountLeft + itUnionRight->surfaceArea() * *itPrimitiveCountRight;
            if(splitIndex == 0 || splitCost < splitCostBest[splitAxis]) {
                splitIndexBest[splitAxis] = splitIndex;
                splitCostBest[splitAxis] = splitCost;
                boundingBoxLeft[splitAxis] = *itUnionLeft;
                boundingBoxRight[splitAxis] = *itUnionRight;
            }
            splitIndex++;
        }
    }
    
    // find the acis which yeilded the best cost
    int splitAxisBest;
    float splitCostBestAxis;
    for(int i = 0; i < 3; i++) {
        if(i == 0 || splitCostBest[i] < splitCostBestAxis) {
            splitAxisBest = i;
            splitCostBestAxis = splitCostBest[i];
        }
    }
    
    // split the objects between the two child notes
    std::vector<ShadableObject*> objectsLeft(sortedObjects[splitAxisBest].begin(), sortedObjects[splitAxisBest].begin() + splitIndexBest[splitAxisBest] + 1);
    std::vector<ShadableObject*> objectsRight(sortedObjects[splitAxisBest].begin() + splitIndexBest[splitAxisBest] + 1, sortedObjects[splitAxisBest].end());
    
    BoundedNode* nodeThis = new BoundedNode();
    nodeThis->object = nullptr;
    // recur on split objects
    nodeThis->nodeLeft = buildHierarchy(objectsLeft, nodeThis);
    nodeThis->nodeLeft->boundingBox = boundingBoxLeft[splitAxisBest];
    nodeThis->nodeLeft->nodeParent = nodeThis;
    nodeThis->nodeRight = buildHierarchy(objectsRight, nodeThis);
    nodeThis->nodeRight->boundingBox = boundingBoxRight[splitAxisBest];
    nodeThis->nodeRight->nodeParent = nodeThis;

    return nodeThis;
}

ShadableObjectIntersection BoundedHierarchy::closestIntersection(Ray ray) {
    // BoundedNode's recursive function assumes the passed node is known to intersect the ray.
    // this means we have to check the intersection with the root node before calling it.
    if(root->boundingBox.intersectionTest(ray)) {
        return root->closestIntersection(ray);
    } else {
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
}
