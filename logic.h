#ifndef LOGIC_H
#define LOGIC_H

#include <vector>
#include <string>
#include <QDebug>

#include "drawhandler.h"

using namespace std;

enum Operations{
    READ_FILE,
    DRAW,
    ROTATE,
    MOVE,
    NORMALIZE,
    CLEAR
};

struct Request{
    int operation;
    string path;
    Axis axis;
    float rotation_angle;
    float offset_value;
    float normalization[2];
};

struct Response{
    bool done = true;
    string message;
    int line_count;
    Line* lines;
};

/* Глобальные переменные */


Response* execute(Request* request);

#endif // LOGIC_H
