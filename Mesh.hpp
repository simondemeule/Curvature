//
//  Mesh.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "ShadableObject.hpp"
#include "ShadableAttributes.hpp"
#include "ShadableObjectIntersection.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

// mesh class
class Mesh : public ShadableObject {
private:
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    
    void updateBoundingBox();
public:
    Mesh(std::string filePath, ShadableAttributes shadingAttributesNew);
    
    ShadableObjectIntersection intersection(Ray ray);
};
