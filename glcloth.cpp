
#include "glcloth.h"
#include "vertex.h"





GLCloth::GLCloth(int numX, int numY, float x, float y, float z)
	: verticesB(QOpenGLBuffer::VertexBuffer), indicesB(QOpenGLBuffer::IndexBuffer)
{


	cl1 = new Cloth(numX, numY, x, y, z);



	initializeOpenGLFunctions();

	verticesB.create();
	indicesB.create();

	verticesB.setUsagePattern(QOpenGLBuffer::StaticDraw);
	indicesB.setUsagePattern(QOpenGLBuffer::StaticDraw);


	set_vertex();


}
void GLCloth::set_vertex() {

	verticesB.destroy();
	indicesB.destroy();

	verticesB.create();
	indicesB.create();

	verticesB.setUsagePattern(QOpenGLBuffer::StaticDraw);
	indicesB.setUsagePattern(QOpenGLBuffer::StaticDraw);

	verticesB.bind();

	verticesB.allocate(cl1->vertices.data(), cl1->vertices.size() * sizeof(float));


	verticesB.release();

	indicesB.bind();
	indicesB.allocate(cl1->indices.data(),
		cl1->indices.size() * sizeof(uint32_t));
	indicesB.release();




}
void GLCloth::draw(GLuint vp)
{

	//update_spring();
	set_vertex();

	glColor3f(1.0f, 0.0f, 0.0f);

	verticesB.bind();
	indicesB.bind();


	glVertexAttribPointer(vp, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
	glDrawElements(GL_TRIANGLES, indicesB.size() / sizeof(uint32_t),
		GL_UNSIGNED_INT, NULL);

	verticesB.release();
	indicesB.release();


}
void GLCloth::update_spring()
{

	float timeStep = 6 / 60.0f;	// ÃÊ±â°ª 2/60.0f  Ä¿Áú¼ö·Ï »¡¶óÁü
	cl1->StepPhysics(timeStep);
	

}



void GLCloth::DrawGrid()
{


}

