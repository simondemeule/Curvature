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
    
    std::vector<int> vertexIndices;
    std::vector<int> normalIndices;
    std::vector<int> uvIndices;
    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec3> normalData;
    std::vector<glm::vec2> uvData;
};
