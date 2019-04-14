
#include <QMenuBar>
#include <QTimer>

#include "window.h"
#include "canvas.h"
#include "thcloth.h"

Window::Window(QWidget *parent) :
	QMainWindow(parent),
	
	time(0),
	m_connect(false),
	cloth_action(new QAction("Cloth", this)),

	thread_action1(new QAction("Thread", this)),
	thread_finish1(new QAction("Finish", this)),
	connect_action(new QAction("Connect", this)),
	quit_action(new QAction("Quit", this)),
	
	perspective_action(new QAction("Perspective", this)),
	orthogonal_action(new QAction("Orthographic", this)),
	shaded_action(new QAction("Shaded", this)),
	wireframe_action(new QAction("Wireframe", this))

{
	setWindowTitle("Thread Test");
	setAcceptDrops(true);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(2, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);

	QSurfaceFormat::setDefaultFormat(format);

	canvas = new Canvas(format, this);
	setCentralWidget(canvas);


	thc1 = new ThCloth(this, canvas);

	connect(thc1, SIGNAL(FinishCount(int)), this, SLOT(th1_finished()));
	



	QObject::connect(cloth_action, &QAction::triggered,
		this, &Window::draw_start);

	QObject::connect(thread_action1, &QAction::triggered,
		this, &Window::start_thread1);

	QObject::connect(thread_finish1, &QAction::triggered,
		this, &Window::finish_thread1);

	QObject::connect(connect_action, &QAction::triggered,
		this, &Window::connect_cloth);

	
	quit_action->setShortcut(QKeySequence::Quit);
	QObject::connect(quit_action, &QAction::triggered,
		this, &Window::close);


	auto file_menu = menuBar()->addMenu("Thread");
	file_menu->addAction(cloth_action);
	file_menu->addAction(thread_action1);
	file_menu->addAction(thread_finish1);
	file_menu->addAction(connect_action);
	file_menu->addSeparator();
	thread_action1->setEnabled(false);
	thread_finish1->setEnabled(false);
	connect_action->setEnabled(false);

	auto view_menu = menuBar()->addMenu("View");	
	
	auto projection_menu = view_menu->addMenu("Projection");
	projection_menu->addAction(perspective_action);
	projection_menu->addAction(orthogonal_action);
	auto projections = new QActionGroup(projection_menu);
	for (auto p : { perspective_action, orthogonal_action })
	{
		projections->addAction(p);
		p->setCheckable(true);
	}
	perspective_action->setChecked(true);
	projections->setExclusive(true);
	QObject::connect(projections, &QActionGroup::triggered,
		this, &Window::on_projection);

	auto draw_menu = view_menu->addMenu("Draw Mode");
	draw_menu->addAction(shaded_action);
	draw_menu->addAction(wireframe_action);
	auto drawModes = new QActionGroup(draw_menu);
	for (auto p : { shaded_action, wireframe_action })
	{
		drawModes->addAction(p);
		p->setCheckable(true);
	}
	shaded_action->setChecked(true);
	drawModes->setExclusive(true);

	QObject::connect(drawModes, &QActionGroup::triggered,
		this, &Window::on_drawMode);




	//createDockWindows();

	resize(600, 400);
}
void Window::createDockWindows()
{

	//float	KsStruct = 0.5f, KdStruct = -0.25f;
	//float	KsShear = 0.5f, KdShear = -0.25f;
	//float	KsBend = 0.85f, KdBend = -0.25f;

	//QString str[6] = {"KsStruct","KdStruct","KsShear","KdShear","KsBend","KdBend"};

	//QDockWidget *dock = new QDockWidget(tr(""), this);
	//dock->setAllowedAreas(Qt::RightDockWidgetArea);

	//QWidget *newWidget = new QWidget(this);
	//QGridLayout *docklayout = new QGridLayout();
	//
	//QDoubleSpinBox *qdsb1;
	//QDoubleSpinBox *qdsb2;
	//QDoubleSpinBox *qdsb3;
	//QDoubleSpinBox *qdsb4;
	//QDoubleSpinBox *qdsb5;
	//QDoubleSpinBox *qdsb6;

	//qdsb1 = new QDoubleSpinBox();
	//qdsb2 = new QDoubleSpinBox();
	//qdsb3 = new QDoubleSpinBox();
	//qdsb4 = new QDoubleSpinBox();
	//qdsb5 = new QDoubleSpinBox();
	//qdsb6 = new QDoubleSpinBox();
	//
	//qdsb1->setRange(0.0, 7.5);
	//qdsb1->setValue(1.0);
	//qdsb1->setSingleStep(0.5);
	//docklayout->addWidget(qdsb1, 0, 1);

	//qdsb2->setRange(0.0, 7.5);
	//qdsb2->setValue(1.0);
	//qdsb2->setSingleStep(0.5);
	//docklayout->addWidget(qdsb2, 1, 1);

	//qdsb3->setRange(0.0, 7.5);
	//qdsb3->setValue(1.0);
	//qdsb3->setSingleStep(0.5);
	//docklayout->addWidget(qdsb3, 2, 1);

	//qdsb4->setRange(0.0, 7.5);
	//qdsb4->setValue(1.0);
	//qdsb4->setSingleStep(0.5);
	//docklayout->addWidget(qdsb4, 3, 1);

	//qdsb5->setRange(0.0, 7.5);
	//qdsb5->setValue(1.0);
	//qdsb5->setSingleStep(0.5);
	//docklayout->addWidget(qdsb5, 4, 1);

	//qdsb6->setRange(0.0, 7.5);
	//qdsb6->setValue(1.0);
	//qdsb6->setSingleStep(0.5);
	//docklayout->addWidget(qdsb6, 5, 1);

	//QLabel *label1 = new QLabel(str[0]);
	//docklayout->addWidget(label1, 0, 0);

	//QLabel *label2 = new QLabel(str[1]);
	//docklayout->addWidget(label2, 1, 0);
	//
	//QLabel *label3 = new QLabel(str[2]);
	//docklayout->addWidget(label3, 2, 0);

	//QLabel *label4 = new QLabel(str[3]);
	//docklayout->addWidget(label4, 3, 0);

	//QLabel *label5 = new QLabel(str[4]);
	//docklayout->addWidget(label5, 4, 0);

	//QLabel *label6 = new QLabel(str[5]);
	//docklayout->addWidget(label6, 5, 0);

	//newWidget->setLayout(docklayout);
	//dock->setWidget(newWidget);

	//addDockWidget(Qt::RightDockWidgetArea, dock);
	//

}
void Window::draw_start()
{
	thread_action1->setEnabled(true);
	cloth_action->setEnabled(false);
	canvas->draw_cloth();
}

void Window::connect_cloth()
{
	if (m_connect) {

		canvas->disconnect_cloth();

		m_connect = false;

	}
		
	
	else {

		canvas->connect_cloth();

		m_connect = true;

	}

}
void Window::start_thread1()
{
	
	thc1->start();
	thread_action1->setEnabled(false);
	thread_finish1->setEnabled(true);
	connect_action->setEnabled(true);
	//printf("th1 스레드 시작\n");
}
void Window::finish_thread1()
{
	thc1->stop();
}
void Window::th1_finished()
{
	if (thc1->isFinished() == false)
	{
		return;
	}
	if (thc1->isRunning() == true)
	{
		return;
	}
	
	thread_action1->setEnabled(true);
	thread_finish1->setEnabled(false);
	connect_action->setEnabled(false);
	//printf("th1 스레드 종료\n");

}
void Window::on_projection(QAction* proj)
{
	if (proj == perspective_action)
	{
		canvas->view_perspective();
	}
	else
	{
		canvas->view_orthographic();
	}
}

void Window::on_drawMode(QAction* mode)
{
	if (mode == shaded_action)
	{
		canvas->draw_shaded();
	}
	else
	{
		canvas->draw_wireframe();
	}
}
