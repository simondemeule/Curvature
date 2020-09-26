//
//  Field.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Field.hpp"

#include <glm/glm.hpp>

FieldIntersection Field::intersection(Ray ray) {
    BoundedObjectIntersection intersectionBox = boundingBox.intersection(ray);
    FieldIntersection intersectionField;
    // this is necessary because the bounding box intersection function uniquely allows negative distances. we don't want that here.
    intersectionField.exists = intersectionBox.exists && intersectionBox.distance >= 0;
    if(intersectionBox.exists) {
        intersectionField.object = this;
        intersectionField.distance = intersectionBox.distance;
        intersectionField.origin = ray.origin + ray.direction * intersectionBox.distance;
        intersectionField.incident = ray.direction;
    }
    return intersectionField;
}
