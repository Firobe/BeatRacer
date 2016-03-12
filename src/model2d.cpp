#include "model2d.h"
#include "video.h"
#include <new>
#include <vector>
#include <stdexcept>

using namespace std;

Model2D::Model2D(glm::vec2 screen) : Model(), _screen(screen), _curScale(1., 1.) {
	_shaderNb = 3;
	addUniform("screen", 2);
	addUniform("calpha", 1);
	setUniform("screen", screen.x, 0);
	setUniform("screen", screen.y, 1);
	setUniform("calpha", 1.);
}

Model2D::~Model2D() {
}

void Model2D::setSize(glm::vec2 newSize){
	if(newSize.x == 0 || newSize.y == 0)
		return;
	scale(glm::vec3( newSize.x / _curScale.x, newSize.y / _curScale.y, 0.));
	_curScale = newSize;
}

void Model2D::place(glm::vec2 pos){
	resetMatrix();
	translate(glm::vec3(pos, 0.));
	scale(glm::vec3(_curScale, 0.));
}

void Model2D::loadV() {
	//VBO
	if (glIsBuffer(_vboID) == GL_TRUE)
		glDeleteBuffers(1, &_vboID);

	int size1 = _vertexNb * 2 * sizeof(float);
	int size2 = _vertexNb * 2 * sizeof(float);
	glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, size1 + size2, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size1, _mapModel);
	glBufferSubData(GL_ARRAY_BUFFER, size1, size2, _mapTex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VAO
	if (glIsVertexArray(_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &_vaoID);

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size1));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Model2D::loadModel(string path) {
	_vertexNb = 6;

	_mapModel = new float[_vertexNb * 2];
	_mapTex = new float[_vertexNb * 2];

	_mapModel[0] = 0.; _mapModel[1] = 0.;
	_mapModel[2] = 0.; _mapModel[3] = 1.;
	_mapModel[4] = 1.; _mapModel[5] = 1.;

	_mapModel[6] = 1.; _mapModel[7] = 1.;
	_mapModel[8] = 1.; _mapModel[9] = 0.;
	_mapModel[10] = 0.; _mapModel[11] = 0.;

	for(int i = 0 ; i < 2 * _vertexNb ; i++)
		_mapTex[i] = _mapModel[i];
}

void Model2D::draw(Video& video) {
	video.render2D(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

void Model2D::draw(Video& video, glm::mat4& modelMatrix) {
	video.render2D(_vaoID, _vertexNb, _texture, this, modelMatrix, _shaderNb);
}
