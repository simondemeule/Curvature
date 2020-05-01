//
//  Tile.cpp
//  project
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Tile.hpp"

Tile::Tile(int indexNew, int xNew, int yNew, int xSizeNew, int ySizeNew) : index(indexNew), x(xNew), y(yNew), xSize(xSizeNew), ySize(ySizeNew), data(new glm::vec3[xSizeNew * ySizeNew]) {}

Tile::~Tile() {
    delete [] data;
}

void Tile::write(glm::vec3 color, int xImage, int yImage) {
    int xTile = xImage - x;
    int yTile = yImage - y;
    data[xTile + yTile * xSize] = color;
}

glm::vec3 Tile::read(int xImage, int yImage) {
    int xTile = xImage - x;
    int yTile = yImage - y;
    return data[xTile + yTile * xSize];
}
