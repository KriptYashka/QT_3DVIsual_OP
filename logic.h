#ifndef LOGIC_H
#define LOGIC_H

#include <vector>
#include <string>
#include <QDebug>

#include "drawhandler.h"

using namespace std;

enum Operations{
    LOAD_DATA,
    DRAW,
    ROTATE,
    OFFSET,
    NORMALIZE,
    CLEAR
};

struct Request{
    int operation;
    string fileName;
    Axis axis;
    float rotationAngle;
    float offsetValue;
    float normalization[2];
};

struct Response{
    bool done = true;
    string message;
    int lineCount;
    Line* lines;
};

Response* execute(Request* request);

#endif // LOGIC_H
