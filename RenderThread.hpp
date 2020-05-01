//
//  RenderThread.hpp
//  project
//
//  Created by Simon Demeule on 2019-03-26.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <thread>
#include <mutex>

#include "RenderData.hpp"
#include "Tile.hpp"
#include "Camera.hpp"

class RenderScheduler; // forward declare

// class encapsulating a render thread
class RenderThread {
private:
    RenderScheduler* renderScheduler;
    RenderData* renderData;
    std::thread thread;
    bool done;
    int identifier;
public:
    RenderThread(RenderScheduler* renderSchedulerNew, RenderData* renderDataNew, int identifierNew);
    ~RenderThread();
    
    // start the render loop
    void start();
    
    // wait for the thread to stop executing
    void join();
    
    // the render loop
    void renderLoop();
    
    // tile rendering, which makes up the render loop
    void renderTile(Tile* tile);
};
