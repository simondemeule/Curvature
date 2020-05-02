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
BoundedHierarchy::BoundedHierarchy(std::vector<ShadingObject*> objects) : root(buildHierarchy(objects, nullptr, -1)) {
    root->boundingBox = BoundingBox(root->nodeLeft->boundingBox, root->nodeRight->boundingBox);
}

// manual split axis choice
BoundedNode* BoundedHierarchy::buildHierarchy(std::vector<ShadingObject*> objects, BoundedNode* nodeParent, int splitAxis) {
    if(objects.size() < 1) {
        throw std::runtime_error("Object list is empty");
    } else if(objects.size() == 1) {
        // base case
        BoundedNode *leaf = new BoundedNode();
        ShadingObject* object = objects.at(0);
        leaf->object = object;
        leaf->boundingBox = object->boundingBox;
        leaf->nodeParent = nodeParent;
        leaf->nodeLeft = nullptr;
        leaf->nodeRight = nullptr;
        return leaf;
    }
    // recursive case
    
    // iterate through possible split points and choose the one with minimal SAH cost
    
    glm::vec3 maskAxis(0);
    if(splitAxis == -1) {
        // TODO: automatic split axis selection
        splitAxis = 0;
    }
    if(splitAxis == 0) {
        maskAxis = glm::vec3(1.0, 0.0, 0.0);
    } else if (splitAxis == 1) {
        maskAxis = glm::vec3(0.0, 1.0, 0.0);
    } else if (splitAxis == 2) {
        maskAxis = glm::vec3(0.0, 0.0, 1.0);
    }
    
    // sort objects by position
    // pre-compute left and right bounding boxes for each split point.
    // bounding boxes can be computed in O(N) time rather than O(N^2) if previous bounding boxes are reused, but it's necessary to pre-compute them because left and right work in opposite orders since it's only possible to grow a bounding box.
    // boundingBoxLeft @ split i + 1 = union (boundingBoxLeft @ split i, objectBoundingBox @ i + 1)
    // boundingBoxRight @ split i - 1 = union (boundingBoxRight @ split i, objectBoundingBox @ i - 1)
    
    
    // compute bounding boxes for all possible split points on the chosen axis
    
    std::sort(objects.begin(), objects.end(), [maskAxis](ShadingObject* a, ShadingObject* b) {
        return glm::dot(maskAxis, a->boundingBox.pointPositive) < glm::dot(maskAxis, b->boundingBox.pointPositive);
    });
    
    std::list<BoundingBox> unionLeft;
    std::list<BoundingBox> unionRight;
    std::list<int> primitiveCountLeft;
    std::list<int> primitiveCountRight;
    
    BoundingBox lastBox;
    int lastPrimitiveCount;
    
    // pre-compute left union and primitive count
    for(int i = 0; i < objects.size() - 1; i++) {
        if(i == 0) {
            lastBox = objects.at(i)->boundingBox;
            lastPrimitiveCount = objects.at(i)->primitiveCount;
        } else {
            lastBox = BoundingBox(objects.at(i)->boundingBox, lastBox);
            lastPrimitiveCount += objects.at(i)->primitiveCount;
        }
        unionLeft.push_back(lastBox);
        primitiveCountLeft.push_back(lastPrimitiveCount);
    }
    
    // pre-compute right union and primitive count
    for(int i = objects.size() - 1; i > 0; i--) {
        if(i == objects.size() - 1) {
            lastBox = objects.at(i)->boundingBox;
            lastPrimitiveCount = objects.at(i)->primitiveCount;
        } else {
            lastBox = BoundingBox(objects.at(i)->boundingBox, lastBox);
            lastPrimitiveCount += objects.at(i)->primitiveCount;
        }
        unionRight.push_front(lastBox);
        primitiveCountRight.push_front(lastPrimitiveCount);
    }

    // decide which one is optimal
    float splitCost;
    float splitCostBest;
    int splitIndex = 0;
    int splitIndexBest;
    BoundingBox boundingBoxLeft;
    BoundingBox boundingBoxRight;
    
    std::list<BoundingBox>::iterator itUnionLeft = unionLeft.begin();
    std::list<BoundingBox>::iterator itUnionRight = unionRight.begin();
    std::list<int>::iterator itPrimitiveCountLeft = primitiveCountLeft.begin();
    std::list<int>::iterator itPrimitiveCountRight = primitiveCountRight.begin();
    
    for(; itUnionLeft != unionLeft.end() && itUnionRight != unionRight.end(); ++itUnionLeft, ++itUnionRight, ++itPrimitiveCountLeft, ++itPrimitiveCountRight) {
        splitCost = itUnionLeft->surfaceArea() * *itPrimitiveCountLeft + itUnionRight->surfaceArea() * *itPrimitiveCountRight;
        if(splitIndex == 0 || splitCost < splitCostBest) {
            splitIndexBest = splitIndex;
            splitCostBest = splitCost;
            boundingBoxLeft = *itUnionLeft;
            boundingBoxRight = *itUnionRight;
        }
        splitIndex++;
    }
    
    // split the objects between the two child notes
    std::vector<ShadingObject*> objectsLeft(objects.begin(), objects.begin() + splitIndexBest + 1);
    std::vector<ShadingObject*> objectsRight(objects.begin() + splitIndexBest + 1, objects.end());
    
    // update split axis for next nodes
    splitAxis = (splitAxis + 1) % 3;
    
    BoundedNode* nodeThis = new BoundedNode();
    nodeThis->object = nullptr;
    // recur on split objects
    nodeThis->nodeLeft = buildHierarchy(objectsLeft, nodeThis, splitAxis);
    nodeThis->nodeLeft->boundingBox = boundingBoxLeft;
    nodeThis->nodeLeft->nodeParent = nodeThis;
    nodeThis->nodeRight = buildHierarchy(objectsRight, nodeThis, splitAxis);
    nodeThis->nodeRight->boundingBox = boundingBoxRight;
    nodeThis->nodeRight->nodeParent = nodeThis;

    return nodeThis;
}

Intersection BoundedHierarchy::closestIntersection(Ray ray) {
    return root->closestIntersection(ray);
}
