#ifndef DISTANCE_H
#define DISTANCE_H
//This script is used to calculate the distance between 3D point and line segment---
//Author: Shuaixiang (Shu)---
//Date: Oct 8, 2023---

struct Point3D{
    double x, y, z;
};

double dotProduct(const Point3D& v1, const Point3D& v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double magnitudeSquared(const Point3D& v){
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double distance_point_line(const Point3D& point, const Point3D& segmentStart, const Point3D& segmentEnd){
    // Vector representing the line segment---
    Point3D segmentVector = {segmentEnd.x-segmentStart.x, segmentEnd.y-segmentStart.y, segmentEnd.z-segmentStart.z};

    //Vector from the start of the segment to the point---
    Point3D pointVector = {point.x-segmentStart.x, point.y-segmentStart.y, point.z-segmentStart.z};

    //Calculate the projection of pointVector onto segmentVector---
    double t = dotProduct(pointVector, segmentVector) / magnitudeSquared(segmentVector);

    //Check if the projection point is outside the line segment---
    if(t < 0.0){
        //The point is closest to the start of the segment---
        return std::sqrt(magnitudeSquared(pointVector));
    } 
    else if(t > 1.0){
        //The point is closest to the end of the segment---
        Point3D endToPoint = {point.x-segmentEnd.x, point.y-segmentEnd.y, point.z-segmentEnd.z};
        return std::sqrt(magnitudeSquared(endToPoint));
    } 
    else{
        //The point is closest to a point on the segment---
        Point3D projection = {
            segmentStart.x + t*segmentVector.x,
            segmentStart.y + t*segmentVector.y,
            segmentStart.z + t*segmentVector.z
        };
        Point3D pointToProjection = {point.x-projection.x, point.y-projection.y, point.z-projection.z};
        return std::sqrt(magnitudeSquared(pointToProjection));
    }

}



#endif
