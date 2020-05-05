//
//  MeshData.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class MeshData {
public:
    MeshData(std::string filePath);
    
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
};
