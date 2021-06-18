#include "filehandler.h"

using namespace std;

vector<string>* loadData(string path)
{
    ifstream file;
    file.open(path);

    vector<string>* filedata = new vector<string>;

    string str;

    if (file){
        filedata->reserve(20);
        while (getline(file,  str)){
            filedata->push_back(str);
        }
    }

    file.close();
    filedata->shrink_to_fit();

    return filedata;
}
