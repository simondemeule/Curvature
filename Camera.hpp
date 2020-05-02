//
//  Camera.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

// camera class
class Camera {
public:
    glm::vec3 origin;
    glm::vec3 front;
    glm::vec3 top;
    glm::vec3 right;
    float fieldOfView;
    float focalLength;
    float aspectRatio;
    
    Camera(glm::vec3 originNew, glm::vec3 directionNew, glm::vec3 topNew, float fieldOfViewNew, float focalLengthNew, float aspectRatioNew);
    
    // cast a ray from normalized coordinates
    // input:
    // x -> [-1, -1]
    // y -> [-1 / aspectRatio, 1 / aspectRatio]
    Ray castNormalized(glm::vec2 normalizedCoordinates);
};
