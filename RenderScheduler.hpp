//
//  RenderScheduler.hpp
//  Curvature
//
//  Created by Simon Demeule on 2019-03-26.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <vector>
#include <mutex>
#include <string>

#include "RenderData.hpp"
#include "RenderCore.hpp"

class RenderThread; // forward declare
class Tile;
class Image;

// highest-level class that orchestrates all rendering, delegating Tiles to RenderThreads.
class RenderScheduler {
private:
    std::vector<RenderThread *> renderThreads;
    std::vector<Tile *> tiles;
    std::mutex tileMutex;
    std::mutex debugMutex;
    int nextTileIndex = 0;
    RenderData* renderData;
    RenderCore* renderCore;
public:
    
    RenderScheduler(RenderData* renderDataNew);
    
    // print debug text, accessed by renderThread
    void debugOut(int identifier, std::string message);
    
    // creates a tile with appropriate bounds given the render settings and tile index
    Tile* createTile(int tileIndex);
    
    // get the next tile to be rendered, accessed by renderThread
    Tile* getNextTile();
    
    // render the image and save it (create tiles and render threads, run threads, stitch tiles to image, write to file, delete tiles and render threads)
    void renderAndSaveImage();
};
