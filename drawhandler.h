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
    Point2D point_first;
    Point2D point_second;
};

enum Axis{
    X,
    Y,
    Z
};

void normalize(Point** points, int rows, int cols, float* normalization);
void rotate(Point** points, int rows, int cols, Axis axis, float angle);
void offset(Point** points, int rows, int cols, Axis axis, float offset);
Point** create_matrix(int size);
void free_points(Point** matrix, int rows);
void set_point(Point* point, float x, float y, float z);
void get_values(Point** points, float* x, float* y, float* z, int rows, int cols);
void create_lines(Point** points, Line* lines, int rows, int cols);

#endif // DRAWHANDLER_H
