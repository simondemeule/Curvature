//
//  Tile.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>

// small square of the image allocated to a render thread
class Tile {
    glm::vec3* data;  // left to right, top to bottom
                      // (x, y),            (x + 1, y) ...          (x + xSize - 1, y),
                      // (x, y + 1),        (x + 1, y + 1) ...      (x + xSize - 1, y + 1) ...
                      // (x, ySize - 1),    (x + 1, ySize - 1) ...  (x + xSize - 1, ySize - 1)
public:
    const int index;  // index of the tile in the image's vector of tiles
    const int x;      // mapping of x = 0 in tile to x in image
    const int y;      // mapping of y = 0 in tile to y in image
    const int xSize;
    const int ySize;
    
    Tile(int indexNew, int xNew, int yNew, int xSizeNew, int ySizeNew);
    ~Tile();
    
    // write at image space coordinates
    void write(glm::vec3 color, int xImage, int yImage);
    
    // read at image space coordinates
    glm::vec3 read(int xImage, int yImage);
};
