#ifndef TRACK__H
#define TRACK__H

#include <string>
#include <vector>
#include <sstream>

std::string toSTR(int i);

class Track {
    public:
        Track(int);
        ~Track();
        static std::vector<std::string> List();
        std::vector<std::string>& getDiffList() {
            return _diffList;
            }
        void setDiffID(int did) {
            _diffID = did;
            }
        std::string getNT() {
            return "res/map/" + toSTR(_id) + "_" + toSTR(_diffID) + ".nt";
            }
        std::string getOGG() {
            return "res/songs/" + toSTR(_id) + ".ogg";
            }
        std::string getMAP() {
            return "res/map/" + toSTR(_id) + ".map";
            }
    private:
        int _id, _diffID;
        std::vector<std::string> _diffList;
    };

#endif
