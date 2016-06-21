#include "track.h"
#include <stdexcept>
#include <sstream>
#include <string.h>
#include <fstream>

#ifdef _WIN32
#include "../libs/dirent.h"
#else
#include <dirent.h>
#endif

using namespace std;

std::string toSTR(int i) {
	std::stringstream ss;
    ss << i;
    return ss.str();
    }


Track::Track(int id) : _id(id) {
    ifstream file("res/tracks/" + toSTR(_id) + ".trk");
    string buffer;

    while (buffer != "[Difficulties]") file >> buffer;

    while (file >> buffer)
        _diffList.push_back(buffer);
    }

Track::~Track() {};

vector<string> Track::List() {
    DIR* dir;
    struct dirent* ent;
    dir = opendir("res/tracks/");
    vector<string> res;

    if (dir == NULL) {
        throw runtime_error("Unable to list tracks (" + string(strerror(errno)) + ")");
        }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            ifstream file("res/tracks/" + string(ent->d_name));
            res.push_back("");
            string buffer;

            while (buffer != "[Name]") file >> buffer;

            buffer = "";

            while (buffer != "[Difficulties]") {
                res.back() += buffer + " ";
                file >> buffer;
                }

            res.back().erase(0, 1);
            res.back().erase(res.back().size() - 1, 1);

            }
        }

    closedir(dir);
    return res;
    }
