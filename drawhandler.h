#ifndef DRAWHANDLER_H
#define DRAWHANDLER_H

#include <math.h>

struct Point{
    float x;
    float y;
    float z;
};

struct Point2D{
    float x;
    float y;
};

struct Line{
    Point2D start;
    Point2D end;
};

enum Axis{
    X,
    Y,
    Z
};

void normalize(Point** points, int rows, int cols, float* normalization);

void rotate(Point** points, int rows, int cols, Axis axis, float angle);

void offset(Point** points, int rows, int cols, Axis axis, float offset);

Point** createPointMatrix(int rows, int cols);
void freePointMatrix(Point** matrix, int rows);

void setPoint(Point* point, float x, float y, float z);

void getValues(Point** points, float* x, float* y, float* z, int rows, int cols);
void projectLines(Point** points, Line* lines, int rows, int cols);

#endif // DRAWHANDLER_H
