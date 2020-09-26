//
//  RenderThread.cpp
//  Curvature
//
//  Created by Simon Demeule on 2019-03-26.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "RenderThread.hpp"

#include "RenderScheduler.hpp"

RenderThread::RenderThread(RenderScheduler* renderScheduler, RenderCore* renderCore, int identifier) :
    renderScheduler(renderScheduler),
    renderCore(renderCore),
    identifier(identifier)
{}

RenderThread::~RenderThread() {
    if(!done) {
        thread.join();
    }
}

void RenderThread::start() {
    thread = std::thread(&RenderThread::renderLoop, this);
    renderScheduler->debugOut(identifier, "Start");
}

void RenderThread::join() {
    thread.join();
    renderScheduler->debugOut(identifier, "Join");
}

void RenderThread::renderLoop() {
    done = false;
    while(!done) {
        Tile* tile = renderScheduler->getNextTile();
        if(tile == nullptr) {
            renderScheduler->debugOut(identifier, "Done");
            done = true;
        } else {
            renderScheduler->debugOut(identifier, "Tile " + std::to_string(tile->index));
            renderTile(tile);
        }
    }
}

void RenderThread::renderTile(Tile* tile) {
    // loop top to bottom
    for(int y = tile->y; y < tile->y + tile->ySize; y++) {
        // loop left to right
        for(int x = tile->x; x < tile->x + tile->xSize; x++) {
            // render pixel
            glm::vec3 color = renderCore->colorPixel(x, y);
            // write color to tile
            tile->write(color, x, y);
        }
    }
}
