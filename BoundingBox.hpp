//
//  BoundingBox.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

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
    // sourrounding box constructor
    BoundingBox(BoundingBox &firstBox, BoundingBox &secondBox);
    
    void setPointPositive(glm::vec3 pointPositiveNew);
    void setPointNegative(glm::vec3 pointNegativeNew);
    
    bool intersectionTest(Ray ray);
    bool overlapTest(BoundingBox boundingBox);
    bool containmentTest(Ray ray);
    
    float surfaceArea();
};
