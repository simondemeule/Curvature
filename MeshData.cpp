//
//  MeshData.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "MeshData.hpp"

#include "OBJloader.h"

MeshData::MeshData(std::string filePath) {
    loadOBJ(filePath.c_str(), indices, vertices, normals, UVs);
}
