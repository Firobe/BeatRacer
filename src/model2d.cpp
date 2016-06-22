#include "model2d.h"
#include "video.h"
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
	scale(glm::dvec3( newSize.x / _curScale.x, newSize.y / _curScale.y, 0.));
	_curScale = newSize;
}

void Model2D::place(glm::vec2 pos){
	resetMatrix();
	translate(glm::dvec3(pos, 0.));
	scale(glm::dvec3(_curScale, 0.));
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
	glBufferSubData(GL_ARRAY_BUFFER, 0, size1, _mapModel.data());
	glBufferSubData(GL_ARRAY_BUFFER, size1, size2, _mapTex.data());
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

	_mapModel.resize(_vertexNb * 2);
	_mapTex.resize(_vertexNb * 2);

	quadCoords(_mapModel, 0., 1., 0., 1.);
	quadCoords(_mapTex, 0., 1., 0., 1.);
}

void quadCoords(vector<float>& data, float x1, float x2, float y1, float y2, bool revert){
	if(revert){
		float tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	data[0] = x1; data[1] = y1;
	data[2] = x1; data[3] = y2;
	data[4] = x2; data[5] = y2;

	data[6] = x2; data[7] = y2;
	data[8] = x2; data[9] = y1;
	data[10] = x1; data[11] = y1;
}

void Model2D::draw(Video& video) {
	video.render2D(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

void Model2D::draw(Video& video, glm::dmat4& modelMatrix) {
	video.render2D(_vaoID, _vertexNb, _texture, this, modelMatrix, _shaderNb);
}
