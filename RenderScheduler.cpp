//
//  RenderScheduler.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-26.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "RenderScheduler.hpp"

#include <iostream>
#include <chrono>

#include "RenderThread.hpp"
#include "Tile.hpp"
#include "Image.hpp"
#include "Sphere.hpp"

RenderScheduler::RenderScheduler(RenderData* renderDataNew) : renderData(renderDataNew) {}

void RenderScheduler::debugOut(int identifier, std::string message) {
    std::lock_guard<std::mutex> guard(debugMutex);
    std::cout << "Thread " << identifier << "\t → " << message << std::endl;
}

Tile* RenderScheduler::createTile(int tileIndex) {
    if(tileIndex >= renderData->tileCount || tileIndex < 0) {
        throw std::runtime_error("Invalid tile index");
    }
    int xCount = (tileIndex % renderData->tileCountWidth); // x location of the tile in terms of tiles, not pixels.
    int yCount = (tileIndex / renderData->tileCountWidth); // y location of the tile in terms of tiles, not pixels.
    int xSize;
    int ySize;
    if(xCount == renderData->tileCountWidth - 1) {
        // tile is on the right edge, xSize may be smaller than tileSize if it's truncated
        xSize = renderData->tileSize - (renderData->tileSize * renderData->tileCountWidth - renderData->outputWidth);
    } else {
        // tile is not on the right edge, xSize is equal to tileSize
        xSize = renderData->tileSize;
    }
    if(yCount == renderData->tileCountHeight - 1) {
        // tile is on the lower edge, ySize may be smaller than tileSize if it's truncated
        ySize = renderData->tileSize - (renderData->tileSize * renderData->tileCountHeight - renderData->outputHeight);
    } else {
        // tile is not on the lower edge, ySize is equal to tileSize
        ySize = renderData->tileSize;
    }
    int xPixel = xCount * renderData->tileSize;
    int yPixel = yCount * renderData->tileSize;
    return new Tile(tileIndex, xPixel, yPixel, xSize, ySize);
}

Tile* RenderScheduler::getNextTile() {
    std::lock_guard<std::mutex> guard(tileMutex);
    if(nextTileIndex >= renderData->tileCount) {
        return nullptr; // thread quits when nullptr is returned
    }
    return tiles[nextTileIndex++];
}

void RenderScheduler::renderAndSaveImage() {
    // create tiles
    for(int tileIndex = 0; tileIndex < renderData->tileCount; tileIndex++) {
        tiles.push_back(createTile(tileIndex));
    }
    // create threads
    for(int identifier = 0; identifier < renderData->threadCount; identifier++) {
        renderThreads.push_back(new RenderThread(this, renderData, identifier));
    }
    // start threads
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Starting Render Threads" << std::endl;
    for(int threadIndex = 0; threadIndex < renderThreads.size(); threadIndex++) {
        renderThreads[threadIndex]->start();
    }
    // wait for threads to complete
    for(int threadIndex = 0; threadIndex < renderThreads.size(); threadIndex++) {
        renderThreads[threadIndex]->join();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Done in "<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << std::endl;
    // stitch tiles into image
    std::cout << "Stitching Tiles to Image" << std::endl;
    Image image(renderData->outputWidth, renderData->outputHeight);
    for(int tileIndex = 0; tileIndex < tiles.size(); tileIndex++) {
        Tile* tile = tiles[tileIndex];
        // loop top to bottom
        for(int y = tile->y; y < tile->y + tile->ySize; y++) {
            // loop left to right
            for(int x = tile->x; x < tile->x + tile->xSize; x++) {
                image.write(tile->read(x, y), x, y);
            }
        }
    }
    // delete tiles
    for(int tileIndex = 0; tileIndex < tiles.size(); tileIndex++) {
        delete tiles[tileIndex];
    }
    tiles.clear();
    // delete threads
    for(int threadIndex = 0; threadIndex < renderThreads.size(); threadIndex++) {
        delete renderThreads[threadIndex];
    }
    renderThreads.clear();
    std::cout << "Done" << std::endl;
    image.saveToFile(renderData->workingDirectory + renderData->outputFileName);
}
