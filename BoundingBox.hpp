//
//  BoundingBox.hpp
//  Curvature
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"
#include "BoundedObjectIntersection.hpp"
#include "DistanceMeasure.hpp"

// axis aligned bounding box class
class BoundingBox {
public:
    // point with most negative x y z coordinates
    glm::vec3 pointPositive;
    // point with most positive x y z coordinates
    glm::vec3 pointNegative;

    // default constructor
    BoundingBox();
    // usual constructor
    BoundingBox(glm::vec3 pointPositiveNew, glm::vec3 pointNegativeNew);
    // sourrounding box constructor, creates the smallest bounding box that fits both passed boxes
    BoundingBox(BoundingBox &firstBox, BoundingBox &secondBox);
    
    void setPointPositive(glm::vec3 pointPositiveNew);
    void setPointNegative(glm::vec3 pointNegativeNew);
    
    // intersection function
    BoundedObjectIntersection intersection(Ray ray);
    // distance function
    DistanceMeasure distance(glm::vec3 point);
    // distance function for furthest point on box
    DistanceMeasure distanceEnd(glm::vec3 point);
    // distance function for biggest inscribed circle in box centered on point
    DistanceMeasure distanceInscribed(glm::vec3 point);
    // surface area function
    float surfaceArea();
    
    // test whether the ray intersects with the box
    bool intersectionTest(Ray ray);
    // test whether the passed box lives entirely within the called box, ie calledBox union passedBox = calledBox
    bool containmentTest(BoundingBox boundingBox);
    // test whether the point is in the box
    bool containmentTest(glm::vec3 point);
    // test whether the ray origin is in the box
    bool containmentTest(Ray ray);
    // test whether the passed box has any overlap with the called box, ie passedBox intersection calledBox != empty
    bool overlapTest(BoundingBox boundingBox);
    // test similar to above, but overlap distance must be greater than zero, ie if boxes share precisely the same face they don't count as overlapping
    bool overlapNonZeroTest(BoundingBox boundingBox);
    
};
