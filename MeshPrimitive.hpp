//
//  MeshPrimitive.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadableObject.hpp"
#include "ShadableObjectIntersection.hpp"
#include "DistanceMeasure.hpp"

class MeshInstance; // forward declare

class MeshPrimitive : public ShadableObject {
private:
    void updateGeometryData();
    void updateBoundingBox();
    
    MeshInstance* meshInstance;
    
    int indexLocation;
    
    glm::vec3 corner1;
    glm::vec3 corner2;
    glm::vec3 corner3;
    
    glm::vec3 edge21;
    glm::vec3 edge32;
    glm::vec3 edge13;
    
    glm::vec3 normalTrue;
    
    glm::vec3 normal1;
    glm::vec3 normal2;
    glm::vec3 normal3;
    
    float areaTotal;
public:
    MeshPrimitive(MeshInstance* meshInstanceNew, int indexLocationNew);
    
    // intersection function
    ShadableObjectIntersection intersection(Ray ray);
    // distance function
    DistanceMeasure distance(glm::vec3 point);
};
