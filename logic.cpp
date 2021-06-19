#include "logic.h"
#include "filehandler.h"
#include "drawhandler.h"
#include "metrichandler.h"

void split_string(string input, string* output);
void set_points();

vector<string> *csv_data;

bool is_loaded = false, is_drawed = false, is_created = false;
int matsize = -1;

vector<vector<Point>> matrix;
Line* lines;

float normalization[2] = {50, 400}; // Стандартная нормализация

Response* execute(Request* request)
{
    Response* response = new Response;

    switch (request->operation){
        case Operations::READ_FILE:
        /* Загрузка данных из файла */
            if (is_loaded){
                csv_data->clear();
            }
            csv_data = load_csv_file(request->path);
            is_loaded = csv_data->size() != 0;
            if (!is_loaded){
                response->done = false;
                response->message = "Data error";
            } else {
                matsize = csv_data->size();
                lines = new Line[(matsize - 1) * matsize * 2];

                if (is_created){ // Очищение поля для отрисовки, если матрица уже создана
                    matrix.clear();
                }
                is_created = true;

                set_points();
                matrix = normalize(matrix, matsize, matsize, normalization);
                create_lines(matrix, lines, matsize, matsize);
            }
            break;

        case Operations::CLEAR:
        /* Удаление матрицы */
            matrix.clear();
            delete [] lines;
            break;

        case Operations::DRAW:
        /* Отрисовка точек */
            if (is_loaded){
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Data error";
            }
            break;

        case Operations::ROTATE:
        /* Поворот точек по одной из осей */
            if (is_loaded){
                matrix = rotate(matrix, matsize, matsize, request->axis, request->rotation_angle);
                matrix = normalize(matrix, matsize, matsize, normalization);
                create_lines(matrix, lines, matsize, matsize);
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Data error";
            }
            break;

        case Operations::MOVE:
        /* Перемещение по поверхности */
            if (is_loaded){
                matrix = offset(matrix, matsize, matsize, request->axis, request->offset_value);
                create_lines(matrix, lines, matsize, matsize);

                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Data error";
            }
            break;

        case Operations::NORMALIZE:
        /* Процесс нормализации, сокращение кол-ва точек */
            normalization[0] = request->normalization[0];
            normalization[1] = request->normalization[1];

            if (is_loaded){
                matrix = normalize(matrix, matsize, matsize, normalization);
                create_lines(matrix, lines, matsize, matsize);
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Data error";
            }
            break;
    }

    response->line_count = (matsize - 1) * matsize * 2;
    return response;
}

void split_string(string input, string* output){
    /* Распределение по элементам из файла */
    string delimiter = ",";
    int i = 0;
    size_t pos = 0;
    while ((pos = input.find(delimiter)) != string::npos){
        string str = input.substr(0, pos);
        output[i++] = str;
        input.erase(0, pos + delimiter.length());
    }
    output[i] = input;
}

void set_points(){
    /* Установка точек в 3D пространстве */
    string point_values[matsize];
    for (int i = 0; i < matsize; i++){
        split_string((*csv_data)[i], point_values);
        vector<Point> line;
        for (int j = 0; j < matsize; j++){
            Point point;
            set_axis_point(&point, (float)i, (float)j, (float)atof(point_values[j].c_str()));
            line.push_back(point);
        }
        matrix.push_back(line);
    }
}
