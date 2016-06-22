#include "model.h"
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace glm;

Model::Model() : _modelMatrix(1.) {
	_vboID = 0;
	_vaoID = 0;
	_textured = true;
	_shaderNb = 0;
}

Model::~Model() {
	glDeleteBuffers(1, &_vboID);
	glDeleteVertexArrays(1, &_vaoID);
}

void Model::load(string name) {
	loadTexture(name);
	loadModel(name);
	loadV();
}

glm::dmat4 Model::getMatrix() {
	return _modelMatrix;
}

void Model::loadTexture(string path) {
	_texture.setPath("res/tex/" + path + ".png");

	try{ 
		_texture.load();
	}
	catch (exception const& ex) {
		cout << "WARNING : " << ex.what() << endl;
		_textured = false;
		_texture.setPath("res/tex/default.png");
		_texture.load();
	}
}

void Model::addUniform(string name, int size){
	_uniform.resize(_uniform.size() + size);
	_uniformStructure.emplace_back(size, name);
}


void Model::setUniform(string name, float value, int i){
	unsigned int j = 0;
	unsigned int ind = 0;
	while(j < _uniformStructure.size() && _uniformStructure[j].name != name){
		ind += _uniformStructure[j].size;
		j++;
	}

	if(j >= _uniformStructure.size())
		throw range_error("Uniform variable " + name + " unkown");
	if(i >= _uniformStructure[j].size)
		throw range_error("Out of uniform variable '" + name + "' bounds");
	_uniform[ind + i] = value;
}

void Model::setShaderNb(int nb){
	_shaderNb = nb;
}

void Model::translate(dvec3 tr) {
	_modelMatrix = glm::translate(_modelMatrix, tr);
}

void Model::rotate(double angle, dvec3 axis) {
	_modelMatrix = glm::rotate(_modelMatrix, angle, axis);
}

void Model::scale(dvec3 sc) {
	_modelMatrix = glm::scale(_modelMatrix, sc);
}

void Model::resetMatrix() {
	_modelMatrix = glm::dmat4(1.);
}

void Model::setOrientation(glm::dmat3 axes) {
	_modelMatrix[0] = glm::dvec4(axes[0], 0.);
	_modelMatrix[1] = glm::dvec4(axes[1], 0.);
	_modelMatrix[2] = glm::dvec4(axes[2], 0.);
}

void Model::uniformize(int shaderID){
	unsigned int i = 0;
	for( auto&& u : _uniformStructure ){
		int location = glGetUniformLocation(shaderID, u.name.c_str());
		if(location == -1)
			throw runtime_error("Uniform variable '" + u.name + "' does not exist in the shader");
		switch(u.size){
			case 4:
				glUniform4fv(location, 1, _uniform.data() + i);
				break;
			case 3:
				glUniform3fv(location, 1, _uniform.data() + i);
				break;
			case 2:
				glUniform2fv(location, 1, _uniform.data() + i);
				break;
			case 1:
				glUniform1fv(location, 1, _uniform.data() + i);
				break;
		}
		i += u.size;
	}
}
