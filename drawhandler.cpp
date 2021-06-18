#include "drawhandler.h"
#include "metrichandler.h"

void rotateX(Point* point, float angle);
void rotateY(Point* point, float angle);
void rotateZ(Point* point, float angle);

Point** createPointMatrix(int rows, int cols){
    Point** matrix = new Point* [rows];
    for (int i = 0; i < rows; i++){
        matrix[i] = new Point[cols];
    }
    return matrix;
}

void freePointMatrix(Point** matrix, int rows){
    /* Очистить матрицу */
    if (matrix != nullptr){

    for (int i = 0; i < rows; i++){
        delete [] matrix[i];
    }

    delete matrix;
    }
}

void setPoint(Point* point, float x, float y, float z){
    point->x = x;
    point->y = y;
    point->z = z;
}

void normalize(Point** points, int rows, int cols, float* normalization){
    int size = rows * cols;
    float *valuesX = new float[size];
    float *valuesY = new float[size];
    float *valuesZ = new float[size];

    getValues(points, valuesX, valuesY, valuesZ, rows, cols); // Достаем текущие точки из общей Data

    float minX = findMin(valuesX, size), maxX = findMax(valuesX, size);
    float minY = findMin(valuesY, size), maxY = findMax(valuesY, size);
    float minZ = findMin(valuesZ, size), maxZ = findMax(valuesZ, size);

    float a = *normalization;
    float b = *(normalization + 1);

    float cX = (b - a) / (maxX - minX);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            points[i][j].x = a + ((points[i][j].x - minX) * cX);
            points[i][j].y = a + ((points[i][j].y - minY) * (b - a) / (maxY - minY));
            points[i][j].z = a + ((points[i][j].z - minZ) * (b - a) / (maxZ - minZ));
        }
    }

    delete [] valuesX;
    delete [] valuesY;
    delete [] valuesZ;
}

void rotate(Point** points, int rows, int cols, Axis axis, float angle){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            switch (axis){
                case Axis::X:
                    rotateX(&points[i][j], angle);
                    break;

                case Axis::Y:
                    rotateY(&points[i][j], angle);
                    break;

                case Axis::Z:
                    rotateZ(&points[i][j], angle);
                    break;
            }
        }
    }
}
double pi = 3.14159265;
void rotateX(Point* point, float degrees){
    float angle = degrees / 180.0 * pi;
    point->y = point->y * cos(angle) - point->z * sin(angle);
    point->z = point->z * cos(angle) + point->y * sin(angle);
}

void rotateY(Point* point, float degrees){
    float angle = degrees / 180.0 * pi;
    point->x = point->x * cos(angle) + point->z * sin(angle);
    point->z = point->z * cos(angle) - point->x * sin(angle);
}

void rotateZ(Point* point, float degrees){
    float angle = degrees / 180.0 * pi;
    point->x = point->x * cos(angle) - point->y * sin(angle);
    point->y = point->y * cos(angle) + point->x * sin(angle);
}

void offset(Point** points, int rows, int cols, Axis axis, float offset){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            switch (axis){
                case Axis::X:
                    points[i][j].x += offset;
                    break;

                case Axis::Y:
                    points[i][j].y += offset;
                    break;

                case Axis::Z:
                    points[i][j].z += offset;
                    break;
            }
        }
    }
}

void getValues(Point** points, float* x, float* y, float* z, int rows, int cols){
    int inx = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            x[inx] = points[i][j].x;
            y[inx] = points[i][j].y;
            z[inx] = points[i][j].z;
            inx++;
        }
    }
}

void projectLines(Point** points, Line* lines, int rows, int cols){
    /* Просчет линий, связывающих точек */
    int inx = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols - 1; j++){
            lines[inx].start.x = points[i][j].x;
            lines[inx].start.y = points[i][j].y;
            lines[inx].end.x = points[i][j + 1].x;
            lines[inx].end.y = points[i][j + 1].y;
            inx++;
        }
    }

    for (int i = 0; i < rows - 1; i++){
        for (int j = 0; j < cols; j++){
            lines[inx].start.x = points[i][j].x;
            lines[inx].start.y = points[i][j].y;
            lines[inx].end.x = points[i + 1][j].x;
            lines[inx].end.y = points[i + 1][j].y;
            inx++;
        }
    }
}
