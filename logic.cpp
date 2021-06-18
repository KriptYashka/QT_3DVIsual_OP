#include "logic.h"
#include "filehandler.h"
#include "drawhandler.h"
#include "metrichandler.h"

void splitString(string input, string* output);
void setPoints(Point** points, int rows, int cols);

vector<string> *csv_data;

bool is_loaded = false, is_drawed = false, is_created = false;
int matsize = -1;

Point** points;
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
                if (is_created)  // Очищение поля для отрисовки, если матрица уже создана
                    free_points(points, matsize);

                points = create_matrix(matsize);
                is_created = true;

                setPoints(points, matsize, matsize);
                normalize(points, matsize, matsize, normalization);
                create_lines(points, lines, matsize, matsize);

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

void splitString(string input, string* output){
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

void setPoints(Point** points, int rows, int cols){
    /* Установка точек в 3D пространстве */
    string arr[matsize];

    for (int i = 0; i < rows; i++){
        splitString((*csv_data)[i], arr);

        for (int j = 0; j < cols; j++){
            Point point;
            set_point(&point, (float)i, (float)j, (float)atof(arr[j].c_str()));

            points[i][j] = point;
        }
    }
}
