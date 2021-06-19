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
                response->message = "Не удалось загрузить данные";
            } else {
                matsize = csv_data->size();
                lines = new Line[(matsize - 1) * matsize * 2];

                if (is_created){ // Очищение поля для отрисовки, если матрица уже создана
                    matrix.clear();
                }
                is_created = true;

                set_points();
                normalize(points, matsize, matsize, normalization);
                create_lines(points, lines, matsize, matsize);
            }
            break;

        case Operations::CLEAR:
        /* Удаление матрицы */
            free_points(points, matsize);
            delete [] lines;
            break;

        case Operations::DRAW:
        /* Отрисовка точек */
            if (is_loaded){
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Данные не загружены";
            }
            break;

        case Operations::ROTATE:
        /* Поворот точек по одной из осей */
            if (is_loaded){
                rotate(points, matsize, matsize, request->axis, request->rotation_angle);
                normalize(points, matsize, matsize, normalization);
                create_lines(points, lines, matsize, matsize);
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Данные не загружены";
            }
            break;

        case Operations::MOVE:
        /* Перемещение по поверхности */
            if (is_loaded){
                offset(points, matsize, matsize, request->axis, request->offset_value);
                create_lines(points, lines, matsize, matsize);

                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Данные не загружены";
            }
            break;

        case Operations::NORMALIZE:
        /* Процесс нормализации, сокращение кол-ва точек */
            normalization[0] = request->normalization[0];
            normalization[1] = request->normalization[1];

            if (is_loaded){
                normalize(points, matsize, matsize, normalization);
                create_lines(points, lines, matsize, matsize);
                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Значения нормализации установлены. Данные не загружены";
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
    matrix.clear();
    for (int i = 0; i < matsize; i++){
        split_string((*csv_data)[i], point_values);
        for (int j = 0; j < matsize; j++){
            Point point;
            set_axis_point(&point, (float)i, (float)j, (float)atof(point_values[j].c_str()));
            matrix.at(i).push_back(point);
        }
    }
}
