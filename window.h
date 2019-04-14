#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileSystemWatcher>
#include <QCollator>

class Canvas;
class ThCloth;

class Window : public QMainWindow
{
	Q_OBJECT
public:
	explicit Window(QWidget* parent = 0);

public slots:
	
	void draw_start();
	void start_thread1();
	void finish_thread1();
	void th1_finished();
	void connect_cloth();

private slots:
	void on_projection(QAction* proj);
	void on_drawMode(QAction* mode);


private:
	
	int time;
	bool m_connect;

	QAction* const thread_action1;
	QAction* const thread_finish1;
	QAction* const connect_action;

	QAction* const cloth_action;
	QAction* const quit_action;
	QAction* const perspective_action;
	QAction* const orthogonal_action;
	QAction* const shaded_action;
	QAction* const wireframe_action;


	Canvas* canvas;
	QTimer* timer;

	ThCloth* thc1;

	void createDockWindows();

	

};

#endif // WINDOW_H
