//
//  Camera.cpp
//  project
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Camera.hpp"

#include <math.h>

Camera::Camera(glm::vec3 originNew, glm::vec3 frontNew, glm::vec3 topNew, float fieldOfViewNew, float focalLengthNew, float aspectRatioNew) :
    origin(originNew),
    front(glm::normalize(frontNew)),
    top(glm::normalize(topNew)),
    right(glm::normalize(glm::cross(frontNew, topNew))),
    fieldOfView(fieldOfViewNew),
    focalLength(focalLengthNew),
    aspectRatio(aspectRatioNew)
{}

// cast a ray from normalized coordinates
// input:
// x -> [-1, -1]
// y -> [-1 / aspectRatio, 1 / aspectRatio]
Ray Camera::castNormalized(glm::vec2 normalizedCoordinates) {
    static float scalar = std::tanf(fieldOfView / 360.0 * M_PI) * aspectRatio;
    Ray ray;
    ray.origin = origin;
    ray.direction = glm::normalize(
       front +
       right * (normalizedCoordinates.x * scalar) +
       top * (normalizedCoordinates.y * scalar)
    );
    ray.isCameraRay = true;
    return ray;
}
