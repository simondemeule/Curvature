//
//  BoundingBoxHierarchy.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <vector>
#include <list>
#include <stdexcept>

#include "BoundedNode.hpp"
#include "DistanceMeasure.hpp"
#include "Ray.hpp"

// a BVH tree
template <class Object, class ObjectIntersection>
class BoundedHierarchy {
private:
    // root node
    BoundedNode<Object, ObjectIntersection>* root;
    
    // constructor subroutine
    BoundedNode<Object, ObjectIntersection>* buildHierarchy(std::vector<Object*> objects, BoundedNode<Object, ObjectIntersection>* nodeParent) {
        // inspired by https://medium.com/@bromanz/how-to-create-awesome-accelerators-the-surface-area-heuristic-e14b5dec6160
        
        // TODO: do the sorting once, then remove the objects that get picked progressively so that the sorting isn't repeated at every step
        // TODO: make this multithreaded
        
        if(objects.size() < 1) {
            BoundedNode<Object, ObjectIntersection> *leaf = new BoundedNode<Object, ObjectIntersection>();
            leaf->object = nullptr;
            leaf->nodeParent = nodeParent;
            leaf->nodeLeft = nullptr;
            leaf->nodeRight = nullptr;
            return leaf;
        } else if(objects.size() == 1) {
            // base case
            BoundedNode<Object, ObjectIntersection> *leaf = new BoundedNode<Object, ObjectIntersection>();
            Object* object = objects.at(0);
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
        std::vector<Object*> sortedObjects[3];
        
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
            
            std::sort(sortedObjects[splitAxis].begin(), sortedObjects[splitAxis].end(), [maskAxis](Object* a, Object* b) {
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
        std::vector<Object*> objectsLeft(sortedObjects[splitAxisBest].begin(), sortedObjects[splitAxisBest].begin() + splitIndexBest[splitAxisBest] + 1);
        std::vector<Object*> objectsRight(sortedObjects[splitAxisBest].begin() + splitIndexBest[splitAxisBest] + 1, sortedObjects[splitAxisBest].end());
        
        BoundedNode<Object, ObjectIntersection>* nodeThis = new BoundedNode<Object, ObjectIntersection>();
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
public:
    // create hierarchy
    BoundedHierarchy(std::vector<Object*> objects) {
        // must set root bounding box after the hierarchy is built since this is normally called by the parent
        if(objects.size() == 0) {
            // empty hierarchy
            root = nullptr;
        } else {
            // build hierarchy recursively
            root = buildHierarchy(objects, nullptr);
            // set bounding box for root node
            if(root->nodeLeft != nullptr && root->nodeRight != nullptr) {
                // case where root is internal
                root->boundingBox = BoundingBox(root->nodeLeft->boundingBox, root->nodeRight->boundingBox);
            } else {
                // case where root is leaf
                root->boundingBox = root->object->boundingBox;
            }
        }
        
    }
    
    ObjectIntersection closestIntersection(Ray ray) {
        if(root == nullptr) {
            // case where hirearchy is empty
            ObjectIntersection intersection;
            intersection.exists = false;
            return intersection;
        } else if (root->nodeLeft == nullptr && root->nodeRight == nullptr) {
            // case where root is leaf
            if(root->boundingBox.intersectionTest(ray)) {
                return root->object->intersection(ray);
            } else {
                ObjectIntersection intersection;
                intersection.exists = false;
                return intersection;
            }
        } else {
            // case where root is internal
            
            // BoundedNode's recursive function assumes the passed node is known to intersect the ray.
            // this means we have to check the intersection with the root node before calling it.
            if(root->boundingBox.intersectionTest(ray)) {
                return root->closestIntersection(ray);
            } else {
                ObjectIntersection intersection;
                intersection.exists = false;
                return intersection;
            }
        }
    }
    
    DistanceMeasure distance(glm::vec3 point) {
        if(root == nullptr) {
            // case where hirearchy is empty
            DistanceMeasure distanceMeasure;
            distanceMeasure.origin = point;
            distanceMeasure.distance = std::numeric_limits<float>::infinity();
            return distanceMeasure;
        } else if (root->nodeLeft == nullptr && root->nodeRight == nullptr) {
            // case where root is leaf
            return root->object->distance(point);
        } else {
            // case where root is internal
            return root->distance(point);
        }
    }
    
    std::list<Object*> encompassingObjects(glm::vec3 point) {
        if(root == nullptr) {
            // case where hirearchy is empty
            std::list<Object*> list = {};
            return list;
        } else if (root->nodeLeft == nullptr && root->nodeRight == nullptr) {
            // case where root is leaf
            if(root->boundingBox.containmentTest(point)) {
                std::list<Object*> list = {root->object};
                return list;
            } else {
                std::list<Object*> list = {};
                return list;
            }
        } else {
            // case where root is internal
            
            // BoundedNode's recursive function assumes the passed node is known to contain the point.
            // this means we have to check containment with the root node before calling it.
            if(root->boundingBox.containmentTest(point)) {
                return root->encompassingObjects(point);
            } else {
                std::list<Object*> list = {};
                return list;
            }
        }
    }
};
