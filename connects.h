
#include <QThread>

class Cloth;
class GLCloth;

class GLConnect : public QThread
{

	Q_OBJECT

public:
	GLConnect(QObject *parent, GLCloth* GLC1, GLCloth* GLC2, int connect_int);
	~GLConnect();

	void mainconnect();
	void subconnect();
	void leftconnect();
	void rightconnect();
	void dontover();
	
	void stop();

	

private:
	void run();
	int main_connect, sub_connect, left_connect, right_connect;
	int stopx, stopy, numx, numy;
	int connectInt;

	bool m_running;

	GLCloth* glcs1;
	GLCloth* glcs2;
	Cloth* cls1;
	Cloth* cls2;

};
