#ifndef DRAWHANDLER_H
#define DRAWHANDLER_H
using namespace std;
#include <math.h>
#include <vector>

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

vector<vector<Point>> normalize(vector<vector<Point>> points, int rows, int cols, float* normalization);
vector<vector<Point>> rotate(vector<vector<Point>> points, int rows, int cols, Axis axis, float angle);
vector<vector<Point>> offset(vector<vector<Point>> points, int rows, int cols, Axis axis, float offset);
void set_axis_point(Point* point, float x, float y, float z);
void get_values(vector<vector<Point>> points, float* x, float* y, float* z, int rows, int cols);
void create_lines(vector<vector<Point>> points, Line* lines, int rows, int cols);

#endif // DRAWHANDLER_H
