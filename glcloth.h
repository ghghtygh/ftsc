#ifndef GLCLOTH_H
#define GLCLOTH_H

#include <QTimer>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QtOpenGL>
#include <GL/glu.h>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include "Cloth.h"

class Cloth;

class GLCloth : protected QOpenGLFunctions
{

public:
	GLCloth(int numX, int numY, float x, float y, float z);
	void set_vertex();
	void draw(GLuint vp);

	Cloth* cl1;

public slots:
	void update_spring();


private:
	void DrawGrid();

	QOpenGLBuffer verticesB;
	QOpenGLBuffer indicesB;




};

#endif
