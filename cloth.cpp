


#include "cloth.h"

#include <conio.h>
#include <iostream>


Cloth::Cloth(int numX, int numY, float x1, float y1, float z1)
	:numx(numX), numy(numY), u(numX + 1), v(numY + 1), x2(x1), y2(y1), z2(z1)
{
	stopx = 0;
	stopy = numx;
	
	size1 = 0.14*numy;
	
	hsize = size1 / 2.0f;
	
	total_points = (numx + 1)*(numy + 1);

	FillAll();

}
void Cloth::FillAll() {

	int i = 0, j = 0, count = 0;
	int l1 = 0, l2 = 0;
	float ypos = 7.0f;


	indices.resize(numx*numy * 2 * 3);

	X.resize(total_points);
	V.resize(total_points);
	F.resize(total_points);

	//fill in X
	count = 0;
	for (j = 0; j < v; j++) {

		for (i = 0; i < u; i++) {


			X[count++] = glm::vec3(((((float)(i) / (u - 1)) * size1) + x2), y2 - (((float(j) / (v - 1))* size1)), z2);

		}
	}
	//fill in V

	memset(&(V[0].x), 0, (total_points) * sizeof(glm::vec3));

	//fill in indices

	GLuint* id = &indices[0];
	count = 0;
	for (i = 0; i < numy; i++) {

		for (j = 0; j < numx; j++) {

			int i0 = i * (numx + 1) + j;
			int i1 = i0 + 1;
			int i2 = i0 + (numx + 1);
			int i3 = i2 + 1;

			if ((j + i) % 2) {
				*id++ = i0; *id++ = i2; *id++ = i1;
				*id++ = i1; *id++ = i2; *id++ = i3;
			}
			else {
				*id++ = i0; *id++ = i2; *id++ = i3;
				*id++ = i0; *id++ = i3; *id++ = i1;
			}
		}
	}

	// Horizontal
	for (l1 = 0; l1 < v; l1++) {	// v
		for (l2 = 0; l2 < (u - 1); l2++) {

			AddSpring((l1 * u) + l2, (l1 * u) + l2 + 1, KsStruct, KdStruct, STRUCTURAL_SPRING);
		}         // 가로줄에 대해서 스프링 생성			
	}
	// Vertical
	for (l1 = 0; l1 < (u); l1++) {
		for (l2 = 0; l2 < (v - 1); l2++) {

			AddSpring((l2 * u) + l1, ((l2 + 1) * u) + l1, KsStruct, KdStruct, STRUCTURAL_SPRING);
		}			// 세로줄에 대해서 스프링 생성
	}

	// Shearing Springs
	for (l1 = 0; l1 < (v - 1); l1++)
	{
		for (l2 = 0; l2 < (u - 1); l2++) {

			AddSpring((l1 * u) + l2, ((l1 + 1) * u) + l2 + 1, KsShear, KdShear, SHEAR_SPRING);
			AddSpring(((l1 + 1) * u) + l2, (l1 * u) + l2 + 1, KsShear, KdShear, SHEAR_SPRING);
		}
	}

	for (l1 = 0; l1 < (v); l1++) {
		for (l2 = 0; l2 < (u - 2); l2++) {

			AddSpring((l1 * u) + l2, (l1 * u) + l2 + 2, KsBend, KdBend, BEND_SPRING);
		}

		AddSpring((l1 * u) + (u - 3), (l1 * u) + (u - 1), KsBend, KdBend, BEND_SPRING);
	}

	for (l1 = 0; l1 < (u); l1++) {

		for (l2 = 0; l2 < (v - 2); l2++) {

			AddSpring((l2 * u) + l1, ((l2 + 2) * u) + l1, KsBend, KdBend, BEND_SPRING);
		}

		AddSpring(((v - 3) * u) + l1, ((v - 1) * u) + l1, KsBend, KdBend, BEND_SPRING);
	}

	AddVertices();

}
void Cloth::AddVertices() {

	vertices.clear();

	for (int i = 0; i < X.size(); i++) {

		vertices.push_back(X[i].x);
		vertices.push_back(X[i].z);
		vertices.push_back(X[i].y);
	}
}
void Cloth::AddSpring(int a, int b, float ks, float kd, int type) {

	Spring spring;
	spring.p1 = a;
	spring.p2 = b;
	spring.Ks = ks;
	spring.Kd = kd;
	spring.type = type;

	glm::vec3 deltaP = X[a] - X[b];
	spring.rest_length = sqrt(glm::dot(deltaP, deltaP));	// 절대값
	springs.push_back(spring);


}

void Cloth::StepPhysics(float dt) {

	ComputeForces();

	IntegrateEuler(dt);				// 다음 위치 지정, 땅 판단

	ApplyProvotDynamicInverse();	// 스프링 길이 판단

	AddVertices();

}
void Cloth::ComputeForces() {


	size_t i = 0;

	for (i = 0; i < total_points; i++) {

		F[i] = glm::vec3(0);

		//add gravity force only for non-fixed points

		// if (i != 0 && i != numX)
		if (i != stopx && i != stopy)	// 고정되지 않은 점 확인
			F[i] += gravity;	// 중력 더함

		F[i] += DEFAULT_DAMPING * V[i];	// 속도 감소를 위한 힘
	}

	//add spring forces
	for (i = 0; i < springs.size(); i++) {

		glm::vec3 p1 = X[springs[i].p1];
		glm::vec3 p2 = X[springs[i].p2];

		glm::vec3 v1 = V[springs[i].p1];
		glm::vec3 v2 = V[springs[i].p2];

		glm::vec3 deltaP = p1 - p2;
		glm::vec3 deltaV = v1 - v2;

		float dist = glm::length(deltaP);


		float leftTerm = -springs[i].Ks * (dist - springs[i].rest_length);
		float rightTerm = springs[i].Kd * (glm::dot(deltaV, deltaP) / dist);


		glm::vec3 springForce = (leftTerm + rightTerm)*glm::normalize(deltaP);


		//고정된 점을 제외한 모든 스프링에 힘 추가
		if (springs[i].p1 != stopx && springs[i].p1 != stopy)
		{
			F[springs[i].p1] += springForce;
		}
		if (springs[i].p2 != stopx && springs[i].p2 != stopy)
		{
			F[springs[i].p2] -= springForce;
		}


	}


}
void Cloth::IntegrateEuler(float dt) {

	float deltaTimeMass = dt / mass;	  // mass = 0.5

	size_t i = 0;

	for (i = 0; i < total_points; i++) {
		glm::vec3 oldV = V[i];
		V[i] += (F[i] * deltaTimeMass);
		X[i] += dt * oldV;

		if (X[i].y < -1) {
			X[i].y = -1;
		}
	}

}
void Cloth::ApplyProvotDynamicInverse() {

	for (size_t i = 0; i < springs.size(); i++) {
		//check the current lengths of all springs

		//스프링 개수만큼 반복



		glm::vec3 p1 = X[springs[i].p1];
		glm::vec3 p2 = X[springs[i].p2];
		glm::vec3 deltaP = p1 - p2;


		float dist = glm::length(deltaP);

		if (dist > springs[i].rest_length) {
			// 좌표사이의 거리가 너무 많이 늘어났을때

			dist -= (springs[i].rest_length);
			dist /= 2.0f;
			deltaP = glm::normalize(deltaP);	// deltaP의 크기가 1이 되도록 크기를 조정함
			deltaP *= dist;

			//if (springs[i].p1 >= 0 && springs[i].p1 <= numX)
			if (springs[i].p1 == stopx || springs[i].p1 == stopy) {
				// springs[i].p1이 고정된 점일 때

				V[springs[i].p2] += deltaP;

			}
			//else if (springs[i].p2 >= 0 && springs[i].p2 <= numX)
			else if (springs[i].p2 == stopx || springs[i].p2 == stopy) {
				// springs[i].p2가 고정된 점일 때

				V[springs[i].p1] -= deltaP;

			}
			else {
				// springs[i].p1,p2가 둘다 고정점이 아닐때
				V[springs[i].p1] -= deltaP;
				V[springs[i].p2] += deltaP;
			}
		}
	}


}


Cloth::~Cloth()
{



}