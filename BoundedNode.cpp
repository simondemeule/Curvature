//
//  BoundedNode.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-01.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include <iostream>

#include "BoundedNode.hpp"
#include "BoundedObjectIntersection.hpp"

// utility function that allows tail recursion
ShadableObjectIntersection pickClosest(ShadableObjectIntersection intersectionFirst, ShadableObjectIntersection intersectionSecond) {
    if(!intersectionFirst.exists && !intersectionSecond.exists) {
        // none exist
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    } else if(intersectionFirst.exists && !intersectionSecond.exists) {
        // first exists
        return intersectionFirst;
    } else if(!intersectionFirst.exists && intersectionSecond.exists) {
        // second exists
        return intersectionSecond;
    } else {
        // both exist, return closest
        if(intersectionFirst.distance < intersectionSecond.distance) {
            return intersectionFirst;
        } else {
            return intersectionSecond;
        }
    }
}

ShadableObjectIntersection BoundedNode::incrementDepth(ShadableObjectIntersection intersection, int objectIncrement, int boxIncrement) {
    intersection.objectIntersectionDepth += objectIncrement;
    intersection.boxIntersectionDepth += boxIncrement;
    return intersection;
}

ShadableObjectIntersection BoundedNode::closestIntersection(Ray ray) {
    if(object != nullptr) {
        // base case, node is leaf, call object intersection function
        return incrementDepth(object->intersection(ray), 1, 0);
    } else {
        // recursive case, node is internal
        
        // check intersection with children bounding box first
        BoundedObjectIntersection intersectionBoxLeft = nodeLeft->boundingBox.intersection(ray);
        BoundedObjectIntersection intersectionBoxRight = nodeRight->boundingBox.intersection(ray);
        
        if(!intersectionBoxLeft.exists && !intersectionBoxRight.exists) {
            // none exist
            ShadableObjectIntersection intersection;
            intersection.exists = false;
            return incrementDepth(intersection, 0, 2);
        } else if(intersectionBoxLeft.exists && !intersectionBoxRight.exists) {
            // left exists
            return incrementDepth(nodeLeft->closestIntersection(ray), 0, 2);
        } else if(!intersectionBoxLeft.exists && intersectionBoxRight.exists) {
            // right exists
            return incrementDepth(nodeRight->closestIntersection(ray), 0, 2);
        } else {
            // both exist
            if(nodeLeft->boundingBox.overlapNonZeroTest(nodeRight->boundingBox)) {
                // the bounding boxes overlap, we can't conclude that if there actually is an intersection in the closest node it occludes the other one
                ShadableObjectIntersection intersectionLeft = nodeLeft->closestIntersection(ray);
                ShadableObjectIntersection intersectionRight = nodeRight->closestIntersection(ray);
                if(!intersectionLeft.exists && !intersectionRight.exists) {
                    // none exist
                    ShadableObjectIntersection intersection;
                    intersection.exists = false;
                    return incrementDepth(intersection, intersectionLeft.objectIntersectionDepth + intersectionRight.objectIntersectionDepth, intersectionLeft.boxIntersectionDepth + intersectionRight.boxIntersectionDepth + 2);
                } else if(intersectionLeft.exists && !intersectionRight.exists) {
                    // left exists
                    return incrementDepth(intersectionLeft, intersectionRight.objectIntersectionDepth, intersectionRight.boxIntersectionDepth + 2);
                } else if(!intersectionLeft.exists && intersectionRight.exists) {
                    // right exists
                    return incrementDepth(intersectionRight, intersectionLeft.objectIntersectionDepth, intersectionLeft.boxIntersectionDepth + 2);
                } else {
                    // both exist, return closest
                    ShadableObjectIntersection intersectionClosest = intersectionLeft.distance < intersectionRight.distance ? intersectionLeft : intersectionRight;
                    ShadableObjectIntersection intersectionFurthest = intersectionLeft.distance > intersectionRight.distance ? intersectionLeft : intersectionRight;
                    return incrementDepth(intersectionClosest, intersectionFurthest.objectIntersectionDepth, intersectionFurthest.boxIntersectionDepth + 2);
                }
            } else {
                // the bounding boxes don't overlap, meaning we can conclude that any intersection in the closest node will occlude the other one
                BoundedNode* nodeClosest = intersectionBoxLeft.distance < intersectionBoxRight.distance ? nodeLeft : nodeRight;
                BoundedNode* nodeFurthest = intersectionBoxLeft.distance > intersectionBoxRight.distance ? nodeLeft : nodeRight;
                ShadableObjectIntersection intersectionClosest = nodeClosest->closestIntersection(ray);
                if(intersectionClosest.exists) {
                    // the intersection exists and we can conclude it will occlude any possible intersection in the other node, meaning we can skip computing it
                    return incrementDepth(intersectionClosest, 0, 2);
                } else {
                    // there is no intersection in the closest node, search for one in the other node
                    return incrementDepth(nodeFurthest->closestIntersection(ray), intersectionClosest.objectIntersectionDepth, intersectionClosest.boxIntersectionDepth + 2);
                }
            }
        }
    }
    /*
    if(boundingBox.intersectionTest(ray)) {
        // ray intersects with node
        if(object != nullptr) {
            // node is leaf, use object's method
            return object->intersection(ray);
        } else {
            // there is room for very slight improvement here in the case both boxes have intersections.
            // we can compute the object intersection of the closest node, and only if there is no intersection we then compute the other node's object intersection.
            // this only works if the boxes don't overlap.
            
            // node is internal, delegate to children
            ShadableObjectIntersection intersectionLeft = nodeLeft->closestIntersection(ray);
            ShadableObjectIntersection intersectionRight = nodeRight->closestIntersection(ray);
            if(!intersectionLeft.exists && !intersectionRight.exists) {
                // none exist
                ShadableObjectIntersection intersection;
                intersection.exists = false;
                return intersection;
            } else if(intersectionLeft.exists && !intersectionRight.exists) {
                // left exists
                return intersectionLeft;
            } else if(!intersectionLeft.exists && intersectionRight.exists) {
                // right exists
                return intersectionRight;
            } else {
                // both exist, choose the closest
                if(intersectionLeft.distance < intersectionRight.distance) {
                    return intersectionLeft;
                } else {
                    return intersectionRight;
                }
            }
        }
    } else {
        // ray doesn't intersect with node
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    */
}
