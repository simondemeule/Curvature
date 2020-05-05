//
//  MeshInstance.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "MeshData.hpp"
#include "MeshPrimitive.hpp"
#include "ShadableAttributes.hpp"

class MeshInstance {
public:
    MeshInstance(MeshData* meshData, glm::mat4x4 transformation, ShadableAttributes* shadableAttributes);
    
    MeshData* meshData;
    glm::mat4x4 transformation;
    ShadableAttributes* shadableAttributes;
    
    // creates primitives from mesh data and transformation
    std::vector<MeshPrimitive*> expandPrimitives();
};
