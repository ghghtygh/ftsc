
#include <QtOpenGL>
#include <GL/glu.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <QThread>

class Canvas;
class GLCloth;

class Collision : public QThread
{
	Q_OBJECT

public:
	explicit Collision(QObject *parent, Canvas *canvas);
	

private:
	void run();
	void set_vector();

	bool finish_bool;
	Canvas* canvas;
	
	void circle_collision();

	std::vector<glm::vec3>X,X0,X1,X2,X3,X4,X5;

signals:

public slots:
	void stop();
};

