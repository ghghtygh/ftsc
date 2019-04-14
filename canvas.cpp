#include <QMouseEvent>

#include <cmath>
#include <vector>

#include "canvas.h"
#include "glcloth.h"
#include "connects.h"


Canvas::Canvas(const QSurfaceFormat& format, QWidget *parent)
	: QOpenGLWidget(parent), mesh(nullptr),
	m_connection(0), m_cloth(0), m_collision(0), count(0),
	scale(1), zoom(1), tilt(90), yaw(0),
	perspective(0.25), anim(this, "perspective"), status(""),
	main_connect(1), sub_connect(2), left_connect(3), right_connect(4)
{
	setFormat(format);
	QFile styleFile("C:/Users/homec/source/repos/ftsc_3/ftsc_3/qt/style.qss");
	styleFile.open(QFile::ReadOnly);
	setStyleSheet(styleFile.readAll());

	anim.setDuration(100);
}

Canvas::~Canvas()
{
	makeCurrent();
	doneCurrent();
}

void Canvas::view_anim(float v)
{
	anim.setStartValue(perspective);
	anim.setEndValue(v);
	anim.start();
}

void Canvas::view_orthographic()
{
	view_anim(0);
}

void Canvas::view_perspective()
{
	view_anim(0.25);
}

void Canvas::draw_shaded()
{
	set_drawMode(0);
}

void Canvas::draw_wireframe()
{
	set_drawMode(1);
}

void Canvas::set_status(const QString &s)
{
	status = s;
	update();
}

void Canvas::set_perspective(float p)
{
	perspective = p;
	update();
}

void Canvas::set_drawMode(int mode)
{
	drawMode = mode;
	update();
}

void Canvas::clear_status()
{
	status = "";
	update();
}

void Canvas::initializeGL()
{

	initializeOpenGLFunctions();

	mesh_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/homec/source/repos/ftsc_3/ftsc_3/gl/mesh.vert");
	mesh_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/homec/source/repos/ftsc_3/ftsc_3/gl/mesh.frag");
	mesh_shader.link();

	mesh_wireframe_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/homec/source/repos/ftsc_3/ftsc_3/gl/mesh.vert");
	mesh_wireframe_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/homec/source/repos/ftsc_3/ftsc_3/gl/mesh_wireframe.frag");
	mesh_wireframe_shader.link();


	int numx = 7;
	int numy = 10;

	int numx2 = 3;
	int numy2 = 3;

	float sizea = 0.14 * numy;
	float sizeb = 0.14 * numy2;

	float xa = -0.5f;
	float ya = 1.5f;
	float za = 0.0f;
	
	glc0 = new GLCloth(7, 10, xa, ya, za);
	glc1 = new GLCloth(7, 10, xa, ya, za + 0.3);

	glc2 = new GLCloth(numx2, numy2, xa - sizeb, ya, za);	// 왼쪽 앞
	glc3 = new GLCloth(numx2, numy2, xa - sizeb, ya, za + 0.3);	// 왼쪽 뒤

	glc4 = new GLCloth(numx2, numy2, xa + sizea, ya, za);	// 오른쪽 앞
	glc5 = new GLCloth(numx2, numy2, xa + sizea, ya, za + 0.3);	// 오른쪽 뒤

	con_main = new GLConnect(this, glc0, glc1, main_connect);

	con_sub1 = new GLConnect(this, glc2, glc3, sub_connect);
	con_sub2 = new GLConnect(this, glc4, glc5, sub_connect);

	con_left1 = new GLConnect(this, glc2, glc0, left_connect);
	con_left2 = new GLConnect(this, glc3, glc1, left_connect);

	con_right1 = new GLConnect(this, glc4, glc0, right_connect);
	con_right2 = new GLConnect(this, glc5, glc1, right_connect);

	fps_timer = new QTimer(this);
	connect(fps_timer, SIGNAL(timeout()), this, SLOT(fps_count()));



}
void Canvas::fps_count()
{
	int fpsInt = (count * 50) / 60;

	status = "FPS : " + QString::number(fpsInt);
	
	count = 0;

}

void Canvas::paintGL()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (m_cloth == 1) {

		if (m_connection == 0)
			m_connection = 1;
		
		drawing_cloth();
	}
	
	if (m_connection ==1)	// 한번만 실행시키기 위함
	{
		m_connection++;
		fps_timer->start(1000);
	}

	if (m_collision == 1)
	{

		m_collision++;


	}

	
	

	if (status.isNull())  return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawText(width() - 80, height() - 10, status);




}
void Canvas::connect_cloth()
{
	con_main->start();
	
	con_sub1->start();
	con_sub2->start();

	con_left1->start();
	con_left2->start();

	con_right1->start();
	con_right2->start();

}
void Canvas::disconnect_cloth()
{
	con_main->stop();

	con_sub1->stop();
	con_sub2->stop();

	con_left1->stop();
	con_left2->stop();

	con_right1->stop();
	con_right2->stop();

}
void Canvas::draw_cloth()
{

	m_cloth = 1;
	drawing_cloth();
	update();

}

void Canvas::cloth_updating() {
	
	if (m_cloth != 1)
		m_cloth = 1;

	update();
	
}
void Canvas::drawing_cloth() {


	QOpenGLShaderProgram* selected_mesh_shader = NULL;
	// Set gl draw mode
	if (drawMode == 1)
	{
		selected_mesh_shader = &mesh_wireframe_shader;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		selected_mesh_shader = &mesh_shader;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	selected_mesh_shader->bind();

	// Load the transform and view matrices into the shader
	glUniformMatrix4fv(
		selected_mesh_shader->uniformLocation("transform_matrix"),
		1, GL_FALSE, transform_matrix().data());
	glUniformMatrix4fv(
		selected_mesh_shader->uniformLocation("view_matrix"),
		1, GL_FALSE, view_matrix().data());

	// Compensate for z-flattening when zooming
	glUniform1f(selected_mesh_shader->uniformLocation("zoom"), 1 / zoom);

	// Find and enable the attribute location for vertex position
	const GLuint vp = selected_mesh_shader->attributeLocation("vertex_position");
	glEnableVertexAttribArray(vp);

	// Then draw the mesh with that vertex position
	
	glc0->draw(vp);
	glc1->draw(vp);
	glc2->draw(vp);
	glc3->draw(vp);
	glc4->draw(vp);
	glc5->draw(vp);

	// Reset draw mode for the background and anything else that needs to be drawn
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clean up state machine
	glDisableVertexAttribArray(vp);
	selected_mesh_shader->release();

	count++;
}
QMatrix4x4 Canvas::transform_matrix() const
{
	QMatrix4x4 m;
	m.rotate(tilt, QVector3D(1, 0, 0));
	m.rotate(yaw, QVector3D(0, 0, 1));
	m.scale(-scale, scale, -scale);
	m.translate(-center);
	return m;
}

QMatrix4x4 Canvas::view_matrix() const
{
	QMatrix4x4 m;
	if (width() > height())
	{
		m.scale(-height() / float(width()), 1, 0.5);
	}
	else
	{
		m.scale(-1, width() / float(height()), 0.5);
	}
	m.scale(zoom, zoom, 1);
	m(3, 2) = perspective;
	return m;
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton ||
		event->button() == Qt::RightButton)
	{
		mouse_pos = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton ||
		event->button() == Qt::RightButton)
	{
		unsetCursor();
	}
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	auto p = event->pos();
	auto d = p - mouse_pos;


	if (event->buttons() & Qt::LeftButton)
	{
		yaw = fmod(yaw - d.x(), 360);
		tilt = fmod(tilt - d.y(), 360);
		update();
	}
	else if (event->buttons() & Qt::RightButton)
	{
		center = transform_matrix().inverted() *
			view_matrix().inverted() *
			QVector3D(-d.x() / (0.5*width()),
				d.y() / (0.5*height()), 0);
		update();
	}
	mouse_pos = p;
}


void Canvas::wheelEvent(QWheelEvent *event)
{
	// Find GL position before the zoom operation
	// (to zoom about mouse cursor)
	auto p = event->pos();
	QVector3D v(1 - p.x() / (0.5*width()),
		p.y() / (0.5*height()) - 1, 0);
	QVector3D a = transform_matrix().inverted() *
		view_matrix().inverted() * v;

	if (event->delta() < 0)
	{
		for (int i = 0; i > event->delta(); --i)
			zoom *= 1.001;
	}
	else if (event->delta() > 0)
	{
		for (int i = 0; i < event->delta(); ++i)
			zoom /= 1.001;
	}

	// Then find the cursor's GL position post-zoom and adjust center.
	QVector3D b = transform_matrix().inverted() *
		view_matrix().inverted() * v;
	center += b - a;
	update();
}
void Canvas::update_spring()
{

	glc0->update_spring();
	glc1->update_spring();
	glc2->update_spring();
	glc3->update_spring();
	glc4->update_spring();
	glc5->update_spring();

}
void Canvas::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Canvas::set_vector()
{

}