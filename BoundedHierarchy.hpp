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
#include "Intersection.hpp"
#include "Ray.hpp"

// a BVH tree
class BoundedHierarchy {
private:
    // root node
    BoundedNode* root;
    
    // constructor subroutine
    BoundedNode* buildHierarchy(std::vector<ShadingObject*> objects, BoundedNode* nodeParent, int splitAxis);
public:
    // create hierarchy
    BoundedHierarchy(std::vector<ShadingObject*> objects);
    
    Intersection closestIntersection(Ray ray);
    std::vector<BoundedObject*> overlappingObjects(BoundingBox boundingBox);
};
