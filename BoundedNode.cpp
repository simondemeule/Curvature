//
//  BoundedNode.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-01.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include <iostream>

#include "BoundedNode.hpp"

Intersection BoundedNode::closestIntersection(Ray ray) {
    if(boundingBox.intersectionTest(ray)) {
        // ray intersects with node
        if(object != nullptr) {
            // node is leaf, use object's method
            return object->intersection(ray);
        } else {
            // node is internal, delegate to children
            Intersection intersectionLeft = nodeLeft->closestIntersection(ray);
            Intersection intersectionRight = nodeRight->closestIntersection(ray);
            if(!intersectionLeft.exists && !intersectionRight.exists) {
                // none exist
                Intersection intersection;
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
        Intersection intersection;
        intersection.exists = false;
        return intersection;
    }
}
