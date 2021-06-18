#include "filehandler.h"

using namespace std;

vector<string>* load_csv_file(string path)
{
    ifstream file;
    file.open(path);
    vector<string>* data = new vector<string>;
    string str;
    if (file){
        while (getline(file, str)){
            data->push_back(str);
        }
    }
    file.close();
    data->shrink_to_fit();
    return data;
}
