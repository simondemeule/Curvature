//
//  RenderThread.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-26.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "RenderThread.hpp"

#include "RenderScheduler.hpp"

RenderThread::RenderThread(RenderScheduler* renderSchedulerNew, RenderData* renderDataNew, int identifierNew) :
    renderScheduler(renderSchedulerNew),
    renderData(renderDataNew),
    identifier(identifierNew)
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
            glm::vec3 color(0.0);
            // accumulate anti-aliasing passes
            for(int a = 0; a < renderData->antiAliasingPasses; a++) {
                glm::vec2 normalizedCoordinates = renderData->toNormalizedCoordinates(x, y, a);
                Ray ray = renderData->camera->castNormalized(normalizedCoordinates);
                color += renderData->colorRay(ray);
            }
            // average passes
            color *= 1.0 / renderData->antiAliasingPasses;
            // write color to tile
            tile->write(color, x, y);
        }
    }
}
