#include "drawhandler.h"
#include "metrichandler.h"
using namespace std;

double pi = 3.14159265;

void rotateX(Point* point, float angle);
void rotateY(Point* point, float angle);
void rotateZ(Point* point, float angle);

void set_axis_point(Point* point, float x, float y, float z){
    point->x = x;
    point->y = y;
    point->z = z;
}

vector<vector<Point>> normalize(vector<vector<Point>> matrix, int rows, int cols, float* normalization){
    vector<vector<Point>> res;
    res = matrix;
    int size = rows * cols;
    float *valuesX = new float[size];
    float *valuesY = new float[size];
    float *valuesZ = new float[size];

    get_values(matrix, valuesX, valuesY, valuesZ, rows, cols); // Достаем текущие точки из общей Data

    float minX = findMin(valuesX, size), maxX = findMax(valuesX, size);
    float minY = findMin(valuesY, size), maxY = findMax(valuesY, size);
    float minZ = findMin(valuesZ, size), maxZ = findMax(valuesZ, size);

    float a = normalization[0], b = normalization[1];

    float cX = (b - a) / (maxX - minX);

    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            res[i][j].x = a + ((matrix[i][j].x - minX) * cX);
            res[i][j].y = a + ((matrix[i][j].y - minY) * (b - a) / (maxY - minY));
            res[i][j].z = a + ((matrix[i][j].z - minZ) * (b - a) / (maxZ - minZ));
        }
    }

    delete [] valuesX;
    delete [] valuesY;
    delete [] valuesZ;

    return res;
}

vector<vector<Point>> rotate(vector<vector<Point>> matrix, int rows, int cols, Axis axis, float angle){
    float degrees = angle / 180.0 * pi;
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            switch (axis){
                case Axis::X:
                    matrix[i][j].y = matrix[i][j].y * cos(degrees) - matrix[i][j].z * sin(degrees);
                    matrix[i][j].z = matrix[i][j].z * cos(degrees) + matrix[i][j].y * sin(degrees);
                    break;

                case Axis::Y:
                    matrix[i][j].x = matrix[i][j].x * cos(degrees) + matrix[i][j].z * sin(degrees);
                    matrix[i][j].z = matrix[i][j].z * cos(degrees) - matrix[i][j].x * sin(degrees);
                    break;

                case Axis::Z:
                    matrix[i][j].x = matrix[i][j].x * cos(degrees) - matrix[i][j].y * sin(degrees);
                    matrix[i][j].y = matrix[i][j].y * cos(degrees) + matrix[i][j].x * sin(degrees);
                    break;
            }
        }
    }
    return matrix;
}

vector<vector<Point>> offset(vector<vector<Point>> matrix, int rows, int cols, Axis axis, float offset){
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            switch (axis){
                case Axis::X:
                    matrix[i][j].x += offset;
                    break;

                case Axis::Y:
                    matrix[i][j].y += offset;
                    break;

                case Axis::Z:
                    matrix[i][j].z += offset;
                    break;
            }
        }
    }
    return matrix;
}

void get_values(vector<vector<Point>> points, float* arr_x, float* arr_y, float* arr_z, int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            arr_x[i * rows + j] = points[i][j].x;
            arr_y[i * rows + j] = points[i][j].y;
            arr_z[i * rows + j] = points[i][j].z;
        }
    }
}

void create_lines(vector<vector<Point>> matrix, Line* lines, int rows, int cols){
    /* Просчет линий, связывающих точек */
    int inx = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols - 1; j++){
            lines[inx].point_first.x = matrix[i][j].x;
            lines[inx].point_first.y = matrix[i][j].y;
            lines[inx].point_second.x = matrix[i][j + 1].x;
            lines[inx].point_second.y = matrix[i][j + 1].y;
            inx++;
        }
    }

    for (int i = 0; i < rows - 1; i++){
        for (int j = 0; j < cols; j++){
            lines[inx].point_first.x = matrix[i][j].x;
            lines[inx].point_first.y = matrix[i][j].y;
            lines[inx].point_second.x = matrix[i + 1][j].x;
            lines[inx].point_second.y = matrix[i + 1][j].y;
            inx++;
        }
    }
}
