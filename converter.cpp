#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

#define MILLISECONDS_PER_OPENGL_UNIT (1000./12.)

using namespace std;
//File must be piped through stdin

vector<string> nextLine(string line) {
    vector<string> result;
    stringstream lineStream(line);
    string cell;

    while (getline(lineStream, cell, ',')) {
        result.push_back(cell);
        }

    return result;
    }

int main(int argc, char** argv) {
    string buff;

    while (buff != "[HitObjects]") cin >> buff;

    vector<string> line = nextLine("1");

    while (cin >> buff) {
        line = nextLine(buff);

        if (line.size() == 6 && line[3] == "1") {
            cout << stoi(line[2]) / (double)MILLISECONDS_PER_OPENGL_UNIT << ",";
			cout << pow(2, floor(stoi(line[0]) * 4./512.));
            cout << endl;
            }

        }

    return EXIT_SUCCESS;
    }
