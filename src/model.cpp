#include "model.h"

using namespace std;
using namespace glm;

Model::Model() : _modelMatrix(1.) {
	_vboID = 0;
	_vaoID = 0;
	_textured = true;
	_mapModel = NULL;
	_mapTex = NULL;
	_uniform = NULL;
	_uniformStructure.empty();
	_shaderNb = 0;
}

Model::~Model() {
	glDeleteBuffers(1, &_vboID);
	glDeleteVertexArrays(1, &_vaoID);
	if(_mapModel != NULL)
		delete[] _mapModel;
	if(_mapTex != NULL)
		delete[] _mapTex;
	if(_uniform != NULL)
		delete[] _uniform;
}

void Model::load(string name) {
	loadTexture(name);
	loadModel(name);
	loadV();
}

glm::mat4 Model::getMatrix() {
	return _modelMatrix;
}

void Model::loadTexture(string path) {
	_texture.setPath("res/tex/" + path + ".png");

	try{ 
		_texture.load();
	}
	catch (exception const& ex) {
		cerr << "WARNING : " << ex.what() << endl;
		_textured = false;
		_texture.setPath("res/tex/default.png");
		_texture.load();
	}
}

void Model::loadV() {
	//VBO
	if (glIsBuffer(_vboID) == GL_TRUE)
		glDeleteBuffers(1, &_vboID);

	int size1 = _vertexNb * 3 * sizeof(float);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size1));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Model::loadModel(string path) {
	vector<vec3> vert;
	vector<vec2> tex;
	vector<unsigned int> vInd, tInd;
	path = "res/obj/" + path + ".obj";
	FILE* file = fopen(path.c_str(), "r");

	if (file == NULL)
		throw runtime_error("Unable to open " + path);

	if (!_textured) {
		tex.push_back(vec2(0., 0.));
		tex.push_back(vec2(1., 0.));
		tex.push_back(vec2(1., 1.));
	}

	//Reading file
	for (;;) {
		char lineHeader[128];

		if (fscanf(file, "%s", lineHeader) == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			vec3 temp;
			fscanf(file, "%f %f %f\n", &temp.x, &temp.y, &temp.z);
			vert.push_back(temp);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 temp;
			fscanf(file, "%f %f\n", &temp.x, &temp.y);
			tex.push_back(temp);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int v[3], t[3];

			if (_textured) {
				if (fscanf(file, "%d/%d %d/%d %d/%d\n", &v[0], &t[0], &v[1], &t[1],
							&v[2], &t[2]) != 6)
					throw runtime_error("Unsupported .obj format for " + path);

				vInd.push_back(v[0]);
				vInd.push_back(v[1]);
				vInd.push_back(v[2]);
				tInd.push_back(t[0]);
				tInd.push_back(t[1]);
				tInd.push_back(t[2]);
			}
			else {
				unsigned int v[3];

				if (fscanf(file, "%d %d %d\n", &v[0], &v[1],
							&v[2]) != 3)
					throw runtime_error("Unsupported .obj format for " + path);

				vInd.push_back(v[0]);
				vInd.push_back(v[1]);
				vInd.push_back(v[2]);
				tInd.push_back(1);
				tInd.push_back(2);
				tInd.push_back(3);

			}
		}
	}

	fclose(file);
	_vertexNb = vInd.size();
	_mapModel = new float[_vertexNb * 3];
	_mapTex = new float[_vertexNb * 2];


	//Processing data
	for (unsigned int i = 0 ; i < vInd.size() ; i++) {
		_mapModel[i * 3] = vert[vInd[i] - 1].x;
		_mapModel[i * 3 + 1] = vert[vInd[i] - 1].y;
		_mapModel[i * 3 + 2] = vert[vInd[i] - 1].z;
		_mapTex[i * 2] = tex[tInd[i] - 1].x;
		_mapTex[i * 2 + 1] = tex[tInd[i] - 1].y;
	}

}

void Model::setShaderNb(int nb){
	_shaderNb = nb;
}

void Model::draw(Video& video) {
	video.render(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

void Model::draw(Video& video, glm::mat4& modelMatrix) {
	video.render(_vaoID, _vertexNb, _texture, this, modelMatrix, _shaderNb);
}

void Model::translate(vec3 tr) {
	_modelMatrix = glm::translate(_modelMatrix, tr);
}

void Model::rotate(float angle, vec3 axis) {
	_modelMatrix = glm::rotate(_modelMatrix, angle, axis);
}

void Model::scale(vec3 sc) {
	_modelMatrix = glm::scale(_modelMatrix, sc);
}

void Model::resetMatrix() {
	_modelMatrix = glm::mat4(1.);
}

void Model::setOrientation(glm::mat3 axes) {
	_modelMatrix[0] = glm::vec4(axes[0], 0.);
	_modelMatrix[1] = glm::vec4(axes[1], 0.);
	_modelMatrix[2] = glm::vec4(axes[2], 0.);
}

void Model::uniformize(int shaderID){
	unsigned int i = 0, i2 = 0;
	while( i2 < _uniformStructure.size() ){
		int location = glGetUniformLocation(shaderID, _uniformStructure[i2].name.c_str());
		switch(_uniformStructure[i2].size){
			case 4:
				glUniform4fv(location, 1, _uniform + i);
				break;
			case 3:
				glUniform3fv(location, 1, _uniform + i);
				break;
			case 2:
				glUniform2fv(location, 1, _uniform + i);
				break;
			case 1:
				glUniform1fv(location, 1, _uniform + i);
				break;
		}
		i += _uniformStructure[i2].size;
		i2 ++;
	}
}
