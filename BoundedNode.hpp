//
//  BoundingBoxNode.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-09-28.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BoundedNode.hpp"
#include "BoundedObjectIntersection.hpp"
#include "BoundedObject.hpp"
#include "Ray.hpp"
#include "DistanceMeasure.hpp"

// bounding box node, part of a BVH tree
template <class Object, class ObjectIntersection>
class BoundedNode : public BoundedObject {
public:
    BoundedNode* nodeParent;
    BoundedNode* nodeLeft;
    BoundedNode* nodeRight;
    
    Object* object;
    
    ObjectIntersection incrementDepth(ObjectIntersection intersection, int objectIncrement, int boxIncrement) {
        intersection.objectIntersectionDepth += objectIncrement;
        intersection.boxIntersectionDepth += boxIncrement;
        return intersection;
    }
    
    ObjectIntersection closestIntersection(Ray ray) {
        if(object != nullptr) {
            // base case, node is leaf, call object intersection function
            return incrementDepth(object->intersection(ray), 1, 0);
        } else {
            // recursive case, node is internal
            
            // check intersection with children bounding box first
            BoundedObjectIntersection intersectionBoxLeft = nodeLeft->boundingBox.intersection(ray);
            BoundedObjectIntersection intersectionBoxRight = nodeRight->boundingBox.intersection(ray);
            
            if(!intersectionBoxLeft.exists && !intersectionBoxRight.exists) {
                // none exist
                ObjectIntersection intersection;
                intersection.exists = false;
                return incrementDepth(intersection, 0, 2);
            } else if(intersectionBoxLeft.exists && !intersectionBoxRight.exists) {
                // left exists
                return incrementDepth(nodeLeft->closestIntersection(ray), 0, 2);
            } else if(!intersectionBoxLeft.exists && intersectionBoxRight.exists) {
                // right exists
                return incrementDepth(nodeRight->closestIntersection(ray), 0, 2);
            } else {
                // both exist
                BoundedObjectIntersection intersectionBoxClosest;
                BoundedObjectIntersection intersectionBoxFurthest;
                
                BoundedNode* nodeClosest;
                BoundedNode* nodeFurthest;
                
                if(intersectionBoxLeft.distance < intersectionBoxRight.distance) {
                    intersectionBoxClosest = intersectionBoxLeft;
                    intersectionBoxFurthest = intersectionBoxRight;
                    
                    nodeClosest = nodeLeft;
                    nodeFurthest = nodeRight;
                } else {
                    intersectionBoxClosest = intersectionBoxRight;
                    intersectionBoxFurthest = intersectionBoxLeft;
                    
                    nodeClosest = nodeRight;
                    nodeFurthest = nodeLeft;
                }
                
                ObjectIntersection intersectionClosest = nodeClosest->closestIntersection(ray);
                ObjectIntersection intersectionFurthest = nodeFurthest->closestIntersection(ray);
                
                if(intersectionClosest.exists) {
                    // the closest intersection exists
                    if(false) {//intersectionClosest.distance < intersectionBoxFurthest.distance && intersectionBoxClosest.distance != intersectionBoxFurthest.distance) {
                        // the intersection in the closest node happens before the start of the furthest bounding box, meaning it occludes it. we don't need to compute the full intersection within the furthest node
                        // this also excludes the case where both intersection box distances are equal
                        return incrementDepth(intersectionClosest, 0, 2);
                    } else {
                        // the intersection in the furthest node might be in front of the one in the closest node. we must compute it
                        ObjectIntersection intersectionFurthest = nodeFurthest->closestIntersection(ray);
                        if(!intersectionFurthest.exists || intersectionClosest.distance < intersectionFurthest.distance) {
                            // the furthest intersection doesn't exist, or the closest node has the closest intersection
                            return incrementDepth(intersectionClosest, intersectionFurthest.objectIntersectionDepth, intersectionFurthest.boxIntersectionDepth + 2);
                        } else {
                            // the furthest node has the closest intersection
                            return incrementDepth(intersectionFurthest, intersectionClosest.objectIntersectionDepth, intersectionClosest.boxIntersectionDepth + 2);;
                        }
                    }
                } else {
                    // the closest intersection doesn't exist
                    ObjectIntersection intersectionFurthest = nodeFurthest->closestIntersection(ray);
                    if(intersectionFurthest.exists) {
                        // the furthest exists
                        return incrementDepth(intersectionFurthest, intersectionClosest.objectIntersectionDepth, intersectionClosest.boxIntersectionDepth + 2);
                    } else {
                        // the furthest intersection doesn't exist
                        ObjectIntersection intersection;
                        intersection.exists = false;
                        return incrementDepth(intersection, intersectionClosest.objectIntersectionDepth + intersectionFurthest.objectIntersectionDepth, intersectionClosest.boxIntersectionDepth + intersectionFurthest.boxIntersectionDepth + 2);
                    }
                }
            }
        }
    }
    
    DistanceMeasure incrementDepth(DistanceMeasure distance, int objectIncrement, int boxIncrement) {
        distance.objectDistanceDepth += objectIncrement;
        distance.boxDistanceDepth += boxIncrement;
        return distance;
    }
    
    DistanceMeasure distance(glm::vec3 point) {
        if(object != nullptr) {
            // base case, node is leaf, call object distance function
            DistanceMeasure result = object->distance(point);
            return result;
        } else {
            // recursive case, node is internal
            DistanceMeasure distanceLeftBox = nodeLeft->boundingBox.distance(point);
            DistanceMeasure distanceRightBox = nodeRight->boundingBox.distance(point);
            
            DistanceMeasure distanceClosestBox;
            DistanceMeasure distanceFurthestBox;
            
            BoundedNode* nodeClosest;
            BoundedNode* nodeFurthest;
            
            if(distanceLeftBox.distance < distanceRightBox.distance) {
                distanceClosestBox = distanceLeftBox;
                distanceFurthestBox = distanceRightBox;
                
                nodeClosest = nodeLeft;
                nodeFurthest = nodeRight;
            } else {
                distanceClosestBox = distanceRightBox;
                distanceFurthestBox = distanceLeftBox;
                
                nodeClosest = nodeRight;
                nodeFurthest = nodeLeft;
            }
            
            DistanceMeasure distanceClosest = nodeClosest->distance(point);
            
            if(distanceClosest.distance < distanceFurthestBox.distance) {
                // distance to object in closest node is closer than the start of the other node's bounding box. no need to recur on the furthest node.
                return incrementDepth(distanceClosest, 0, 2);
            } else {
                // we can't optimize here, must compute both full distances
                DistanceMeasure distanceFurthest = nodeFurthest->distance(point);
                if(distanceClosest.distance < distanceFurthest.distance) {
                    return incrementDepth(distanceClosest, distanceFurthest.objectDistanceDepth, distanceFurthest.boxDistanceDepth + 2);
                } else {
                    return incrementDepth(distanceFurthest, distanceClosest.objectDistanceDepth, distanceClosest.boxDistanceDepth + 2);
                }
            }
        }
    }
};
