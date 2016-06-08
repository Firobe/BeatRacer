#include "model3d.h"
#include "video.h"
#include <new>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace glm;

Model3D::Model3D() : Model() {
}

Model3D::~Model3D() {
}

void Model3D::loadV() {
	//VBO
	if (glIsBuffer(_vboID) == GL_TRUE)
		glDeleteBuffers(1, &_vboID);

	int size1 = _vertexNb * 3 * sizeof(float);
	int size2 = _vertexNb * 2 * sizeof(float);
	glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, size1 + size2, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size1, &_mapModel[0]);
	glBufferSubData(GL_ARRAY_BUFFER, size1, size2, &_mapTex[0]);
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

void Model3D::loadModel(string path) {
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
	_mapModel.resize(_vertexNb * 3);
	_mapTex.resize(_vertexNb * 2);


	//Processing data
	for (unsigned int i = 0 ; i < vInd.size() ; i++) {
		_mapModel[i * 3] = vert[vInd[i] - 1].x;
		_mapModel[i * 3 + 1] = vert[vInd[i] - 1].y;
		_mapModel[i * 3 + 2] = vert[vInd[i] - 1].z;
		_mapTex[i * 2] = tex[tInd[i] - 1].x;
		_mapTex[i * 2 + 1] = tex[tInd[i] - 1].y;
	}

}

void Model3D::draw(Video& video) {
	video.render(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

void Model3D::draw(Video& video, glm::dmat4& modelMatrix) {
	video.render(_vaoID, _vertexNb, _texture, this, modelMatrix, _shaderNb);
}
