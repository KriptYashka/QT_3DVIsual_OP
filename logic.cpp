#include "logic.h"
#include "filehandler.h"
#include "drawhandler.h"
#include "metrichandler.h"

int matsize = 20;

void splitString(string input, string* output);
void setPoints(Point** points, int rows, int cols);

vector<string> *loadedData;

bool loaded = false, drawed = false;
bool matrixCreated = false;

Point** points;
Line* lines;

float normalization[2] = {50, 400};

Response* execute(Request* request)
{
    Response* response = new Response;

    switch (request->operation){
        case Operations::LOAD_DATA:
        /* Загрузка данных из файла */
            if (loaded){
                loadedData->clear();
            }

            loadedData = loadData(request->fileName);
            loaded = loadedData->size() != 0;
            matsize = loadedData->size();
            lines = new Line[(matsize - 1) * matsize * 2];
            if (!loaded){
                response->done = false;
                response->message = "Не удалось загрузить данные";
            }
            break;

        case Operations::CLEAR:
        /* Удаление матрицы */
            freePointMatrix(points, matsize);
            delete [] lines;
            break;

        case Operations::DRAW:
        /* Отрисовка точек */
            if (loaded){
                if (matrixCreated)  // Очищение поля для отрисовки, если матрица уже создана
                    freePointMatrix(points, matsize);

                points = createPointMatrix(matsize, matsize);
                matrixCreated = true;

                setPoints(points, matsize, matsize);
                normalize(points, matsize, matsize, normalization);
                projectLines(points, lines, matsize, matsize);

                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Данные не загружены";
            }
            break;

        case Operations::ROTATE:
        /* Поворот точек по одной из осей */
            if (loaded){
                rotate(points, matsize, matsize, request->axis, request->rotationAngle);
                normalize(points, matsize, matsize, normalization);
                projectLines(points, lines, matsize, matsize);

                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Данные не загружены";
            }
            break;

        case Operations::OFFSET:
        /* Перемещение по поверхности */
            if (loaded){
                offset(points, matsize, matsize, request->axis, request->offsetValue);
                projectLines(points, lines, matsize, matsize);

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

            if (loaded){
                normalize(points, matsize, matsize, normalization);
                projectLines(points, lines, matsize, matsize);

                response->lines = lines;
            } else {
                response->done = false;
                response->message = "Значения нормализации установлены. Данные не загружены";
            }
            break;
    }

    response->lineCount = (matsize - 1) * matsize * 2;
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
        splitString((*loadedData)[i], arr);

        for (int j = 0; j < cols; j++){
            Point point;
            setPoint(&point, (float)i, (float)j, (float)atof(arr[j].c_str()));

            points[i][j] = point;
        }
    }
}
