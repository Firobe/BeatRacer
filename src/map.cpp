#include "map.h"
#include "video.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <stdexcept>

enum Axes { xAxis, yAxis, zAxis };

using namespace std;
using namespace glm;

Map::Map() : Model3D() {
}

Map::~Map() {
}

vector<glm::vec3>& Map::getMap() {
	return _transMap;
}

void Map::loadModel(string path) {
	vec3 temp;
	string buffer;
	ifstream map;
	int todo;
	path = "res/map/" + path + ".map";
    map.open(path);
	if(!map)
		throw runtime_error("Unable to open " + path);

	while (getline(map, buffer)) {
		sscanf(buffer.c_str(), "%f,%f,%f:%d", &temp[0], &temp[1], &temp[2], &todo);
		temp[1] = radians(temp[1]);
		temp[2] = -radians(temp[2]);

		for (int i = 0 ; i < todo ; i++)
			_transMap.push_back(temp);
	}

	_segmentMap.resize(_transMap.size());
	_vertexNb = 6 * _transMap.size();
	_mapModel = new float[3 * _vertexNb];
	_mapTex = new float[2 * _vertexNb];

	mat4 tmpmod = mat4(1.);
	float cursor = 0;
	vec4 act(0.); //Current origin

	for (int s = 0 ; s < (int)_transMap.size() ; s++) { //SEGMENT
		_segmentMap[s].origin = vec3(act);
		_segmentMap[s].length = _transMap[s][0];
		_segmentMap[s].xAxis = vec3(tmpmod[xAxis]);
		_segmentMap[s].yAxis = vec3(tmpmod[yAxis]);
		_segmentMap[s].zAxis = vec3(tmpmod[zAxis]);

		fillTex(s, cursor);
		vec4 t = tmpmod[yAxis];
		t *= ROAD_WIDTH;
		//Vertices for first half of the two triangles
		fillModel(6 * s, act - t); //V1 (tmpmod[1] -> Y vector)
		fillModel(6 * s + 5, act - t); //V6
		fillModel(6 * s + 1, act + t);

		act += tmpmod * vec4(toCartesian(_transMap[s]), 0.); //Matricial product of model matrix & orientation vector
		tmpmod = glm::rotate(tmpmod, _transMap[s][1], vec3(0, 0, 1));
		tmpmod = glm::rotate(tmpmod, _transMap[s][2], vec3(0, 1, 0));
		tmpmod = glm::translate(tmpmod, _transMap[s][0] * vec3(1, 0, 0));
		t = tmpmod[yAxis];
		t *= ROAD_WIDTH;

		//vertices for second half
		fillModel(6 * s + 2, act + t);
		fillModel(6 * s + 3, act + t);
		fillModel(6 * s + 4, act - t);
		cursor += 1. / ROAD_WIDTH * _transMap[s][0];
	}
}

void Map::fillModel(int vertex, vec4 v) {
	//writing a vertex into the model array
	_mapModel[vertex * 3] = v[0];
	_mapModel[vertex * 3 + 1] = v[1];
	_mapModel[vertex * 3 + 2] = v[2];
}

void Map::fillTex(int segment, float cursor) {
	//Texture coordinates
	int cur = 12 * segment;
	float rho = 1. / ROAD_WIDTH * _transMap[segment][0];
	_mapTex[cur] = 1.;
	_mapTex[cur + 1] = cursor;
	_mapTex[cur + 2] = 0.;
	_mapTex[cur + 3] = cursor;
	_mapTex[cur + 4] = 0.;
	_mapTex[cur + 5] = cursor + rho;
	_mapTex[cur + 6] = 0.;
	_mapTex[cur + 7] = cursor + rho;
	_mapTex[cur + 8] = 1.;
	_mapTex[cur + 9] = cursor + rho;
	_mapTex[cur + 10] = 1.;
	_mapTex[cur + 11] = cursor;
}

glm::vec3 Map::getWorldCoordinates(glm::vec3 roadCoord) {
	static float curLen = 0.;
	static int seg = 0;

	while (seg - 1 >= 0 && curLen - roadCoord.x >= _segmentMap[seg - 1].length) {
		curLen -= _segmentMap[seg - 1].length;
		seg--;
	}

	while (curLen < roadCoord.x && (unsigned int)seg < _segmentMap.size()) {
		curLen += _segmentMap[seg].length;
		seg++;
	}

	if ((unsigned int)seg >= _segmentMap.size() || seg < 0)
		throw out_of_range("Out of map range");

	return _segmentMap[seg].origin + (roadCoord.x - curLen) * _segmentMap[seg].xAxis
		- roadCoord.y * _segmentMap[seg].yAxis + roadCoord.z * _segmentMap[seg].zAxis;
}

glm::mat3 Map::getWorldOrientation(float xPos) { //RADIANS
	static float curLen = 0.;
	static int seg = 0;

	while (seg - 1 >= 0 && curLen - xPos >= _segmentMap[seg - 1].length) {
		curLen -= _segmentMap[seg - 1].length;
		seg--;
	}

	while (curLen < xPos && (unsigned int)seg < _segmentMap.size()) {
		curLen += _segmentMap[seg].length;
		seg++;
	}

	if ((unsigned int)seg >= _segmentMap.size() || seg < 0)
		throw out_of_range("Out of map range");

	return glm::mat3(_segmentMap[seg].xAxis, _segmentMap[seg].yAxis, _segmentMap[seg].zAxis);
}
