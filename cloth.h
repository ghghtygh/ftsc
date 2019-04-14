

#include <QtOpenGL>
#include <GL/glu.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>



class Cloth
{


public:
	Cloth(int numX, int numY, float x1, float y1, float z1);
	~Cloth();



	struct Spring {
		int p1, p2;
		float rest_length;
		float Ks, Kd;
		int type;
	};


	// TEST
	void OnDraw();

	void AddVertices();
	void StepPhysics(float dt);
	void ComputeForces();
	void IntegrateEuler(float dt);
	void ApplyProvotDynamicInverse();
	void AddSpring(int a, int b, float ks, float kd, int type);
	void FillAll();

	std::vector<GLfloat> vertices;
	//std::vector<GLushort> indices;
	std::vector<GLuint> indices;

	std::vector<Spring> springs;


	int numx, numy;
	int u;
	int v;

	std::vector<glm::vec3>X;
	std::vector<glm::vec3>V;
	std::vector<glm::vec3>F;
	int stopx, stopy;

	float x2, y2, z2;
private:




	int total_points;



	float size1;
	float hsize;

	int oldX = 0, oldY = 0;
	float rX = 15, rY = 0;
	int state = 1;
	float dist = -23;
	const int GRID_SIZE = 10;

	const int STRUCTURAL_SPRING = 0;
	const int SHEAR_SPRING = 1;
	const int BEND_SPRING = 2;

	int spring_count = 0;

	const float DEFAULT_DAMPING = -0.0125f;
	float	KsStruct = 0.5f, KdStruct = -0.25f;
	float	KsShear = 0.5f, KdShear = -0.25f;
	float	KsBend = 0.85f, KdBend = -0.25f;

	float mass = 0.5;

	glm::vec3 gravity = glm::vec3(0.0f, -0.00981f, 0.0f);



};
