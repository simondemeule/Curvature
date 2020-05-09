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

#include "BoundedNode.hpp"
#include "ShadableObjectIntersection.hpp"
#include "DistanceMeasure.hpp"
#include "Ray.hpp"

// a BVH tree
class BoundedHierarchy {
private:
    // root node
    BoundedNode* root;
    
    // constructor subroutine
    BoundedNode* buildHierarchy(std::vector<ShadableObject*> objects, BoundedNode* nodeParent);
public:
    // create hierarchy
    BoundedHierarchy(std::vector<ShadableObject*> objects);
    
    ShadableObjectIntersection closestIntersection(Ray ray);
    DistanceMeasure distance(glm::vec3 point);
};
