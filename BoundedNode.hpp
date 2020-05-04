//
//  BoundingBoxNode.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-09-28.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BoundedObject.hpp"
#include "ShadableObject.hpp"
#include "ShadableObjectIntersection.hpp"
#include "Ray.hpp"

// bounding box node, part of a BVH tree
class BoundedNode : public BoundedObject {
public:
    BoundedNode* nodeParent;
    BoundedNode* nodeLeft;
    BoundedNode* nodeRight;
    
    ShadableObject* object;
    
    ShadableObjectIntersection closestIntersection(Ray ray);
};
