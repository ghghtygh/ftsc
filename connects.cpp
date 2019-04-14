
#include "connects.h"
#include "glcloth.h"

#include <vector>

#include <glm/glm.hpp>

GLConnect::GLConnect(QObject *parent, GLCloth* GLC1, GLCloth* GLC2, int connect_int)
	: QThread(parent),
	connectInt(connect_int),
	main_connect(1), sub_connect(2), left_connect(3), right_connect(4),
	m_running(false)
{
	glcs1 = GLC1;
	glcs2 = GLC2;

	cls1 = glcs1->cl1;
	cls2 = glcs2->cl1;

	stopx = cls1->stopx;
	stopy = cls1->stopy;

	numx = cls1->numx;
	numy = cls1->numy;

}


GLConnect::~GLConnect()
{
}


void GLConnect::run() {

	m_running = true;

	while (m_running) {
		
		if (connectInt==1) {
			mainconnect();
		}
		else if (connectInt==2) {
			subconnect();
		}
		else if (connectInt == 3) {
			leftconnect();
		}
		else if (connectInt == 4) {
			rightconnect();
		}
		
		
		msleep(100);
	}
	

	

}
void GLConnect::stop() {
	
	m_running = false;


}

void GLConnect::dontover() {

	int i = 0;
	int j = 0;

	int point_index = 0;

	for (i = 1; i < numx; i++) {
		for (j = 1; j < numy + 1; j++) {

			if (j == 0) {
				if (i > 3 && i < 7) {
					continue;
				}
			}
			else if (i != 0 && i != numx) {
				continue;
			}

			point_index = (j*((numx)+1)) + i;

			if (cls1->X[point_index].z > cls2->X[point_index].z + 0.01) {
				cls1->F[point_index].z -= 0.01;
				cls2->F[point_index].z += 0.01;
			}


		}
	}

}
void GLConnect::mainconnect()
{



	int i = 0;
	int j = 0;
	int point_index = 0;

	for (i = 0; i < numx + 1; i++) {
		for (j = 0; j < numy + 1; j++) {

			if (i != 0 && i != numx) {
				continue;
			}
			if (j < 3) {
				continue;
			}
			point_index = (j*(numx + 1)) + i;

			glm::vec3 p1 = cls1->X[point_index];

			glm::vec3 p2 = cls2->X[point_index];

			glm::vec3 v1 =  (p1 - p2) / 2.0f;

			cls1->X[point_index] = p1 - v1;

			cls2->X[point_index] = p2 + v1;

			cls1->V[point_index] = glm::vec3(0, 0, 0);
			cls2->V[point_index] = glm::vec3(0, 0, 0);

			glm::vec3 f1 = cls1->F[point_index];
			glm::vec3 f2 = cls2->F[point_index];

			glm::vec3 fs = f1 + f2 / 2.0f;

			cls1->F[point_index] = f1 - fs;
			cls2->F[point_index] = f2 + fs;

		}
	}
}

void GLConnect::subconnect()
{


	int i = 0, j = 0, point_index = 0;

	for (i = 0; i < numx + 1; i++) {
		for (j = 0; j < numy + 1; j++) {

			if (j != 0) {
				//continue;
			}
			if (j != numy) {
				continue;
			}


			point_index = (j*(numx + 1)) + i;

			glm::vec3 p1 = cls1->X[point_index];

			glm::vec3 p2 = cls2->X[point_index];

			glm::vec3 v1 = (p1 - p2) / 2.0f;

			cls1->X[point_index] = p1 - v1;

			cls2->X[point_index] = p2 + v1;

			cls1->V[point_index] = glm::vec3(0, 0, 0);
			cls2->V[point_index] = glm::vec3(0, 0, 0);

			glm::vec3 f1 = cls1->F[point_index];
			glm::vec3 f2 = cls2->F[point_index];

			glm::vec3 fs = f1 + f2 / 2.0f;

			cls1->F[point_index] = f1 - fs;
			cls2->F[point_index] = f2 + fs;


		}
	}


}
void GLConnect::leftconnect()
{
	int i = 0, index1 = 0, index2 = 0;



	for (i = 1; i < numy; i++) {

		index1 = (i*(numx + 1)) - 1;
		index2 = ((i - 1)*(cls2->numx + 1));

		glm::vec3 p1 = cls1->X[index1];
		glm::vec3 p2 = cls2->X[index2];


		glm::vec3 v1 = (p1 - p2) / 2.0f;

		cls1->X[index1] = p1 - v1;

		cls2->X[index2] = p2 + v1;

		cls1->V[index1] = glm::vec3(0, 0, 0);
		cls2->V[index2] = glm::vec3(0, 0, 0);

		glm::vec3 f1 = cls1->F[index1];
		glm::vec3 f2 = cls2->F[index2];

		glm::vec3 fs = f1 + f2 / 2.0f;

		cls1->F[index1] = f1 - fs;
		cls2->F[index2] = f2 + fs;

	}



}
void GLConnect::rightconnect()
{

	int i = 0, index1 = 0, index2 = 0;


	for (i = 0; i < numy; i++) {

		index1 = (i*(numx + 1));	// 메인 천
		index2 = ((i + 1)*(cls2->numx + 1)) - 1;	// 오른쪽 팔

		glm::vec3 p1 = cls1->X[index1];
		glm::vec3 p2 = cls2->X[index2];


		glm::vec3 v1 = (p1 - p2) / 2.0f;


		cls2->X[index2] = cls1->X[index1];

		cls1->V[index1] = glm::vec3(0, 0, 0);
		cls2->V[index2] = glm::vec3(0, 0, 0);

		/*
		glm::vec3 f1 = cls1->F[index1];
		glm::vec3 f2 = cls2->F[index2];

		glm::vec3 fs = f1 + f2 / 2.0f;

		cls1->F[index1] = f1 - fs;
		cls2->F[index2] = f2 + fs;
		*/

	}


}



