//
//  MeshInstance.cpp
//  Curvature
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "MeshInstance.hpp"

MeshInstance::MeshInstance(MeshData* meshData, glm::mat4x4 transformation, ShadableAttributes* shadableAttributes) :
    meshData(meshData),
    transformation(transformation),
    shadableAttributes(shadableAttributes)
{}

std::vector<MeshPrimitive*> MeshInstance::expandPrimitives() {
    std::vector<MeshPrimitive*> primitives(meshData->vertexIndices.size() / 3);
    for(int i = 0; i < meshData->vertexIndices.size() / 3; i++) {
        primitives.at(i) = new MeshPrimitive(this, i * 3);
    }
    return primitives;
}
