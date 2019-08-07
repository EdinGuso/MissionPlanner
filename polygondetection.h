// A C++ program to check if a given point lies inside a given polygon
// Refer https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of functions onSegment(), orientation() and doIntersect()

#ifndef POLYGONDETECTION_H
#define POLYGONDETECTION_H

#include <QVector>
#include <QPoint>

struct Point
{
    int x;
    int y;
};

int max(int a, int b);

int min(int a, int b);

bool onSegment(Point p, Point q, Point r);

int orientation(Point p, Point q, Point r);

bool doIntersect(Point p1, Point q1, Point p2, Point q2);

// Returns true if the point p lies inside the polygon[] with n vertices
bool isInsideReal(Point polygon[], int n, Point p);

bool isInsideHelper(QVector<QPointF> pointsQt, QPointF pointQt);

#endif // POLYGONDETECTION_H
