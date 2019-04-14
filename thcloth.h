



#include <QThread>

class Canvas;

class ThCloth : public QThread
{
	Q_OBJECT

public:
	explicit ThCloth(QObject *parent, Canvas *canvas);


private:
	void run();
	bool finish_bool;
	Canvas* canvas;
signals:
	void FinishCount(const int value);

public slots:
	void stop();
};

