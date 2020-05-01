//
//  Mesh.hpp
//  project
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "ShadingObject.hpp"
#include "ShadingAttributes.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

// mesh class
class Mesh : public ShadingObject {
private:
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    
    void updateBoundingBox();
public:
    Mesh(std::string filePath, ShadingAttributes shadingAttributesNew);
    
    Intersection intersection(Ray ray);
};
