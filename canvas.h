#ifndef CANVAS_H
#define CANVAS_H



#include <QtOpenGL>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>
#include <QTimer>


class GLConnect;
class GLMesh;
class Mesh;
class Backdrop;
class GLCloth;

class Canvas : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit Canvas(const QSurfaceFormat& format, QWidget* parent = 0);
	~Canvas();

	void view_orthographic();
	void view_perspective();
	void draw_shaded();
	void draw_wireframe();

	GLCloth* glc[6];

	GLCloth* glc0;
	GLCloth* glc1;
	GLCloth* glc2;
	GLCloth* glc3;
	GLCloth* glc4;
	GLCloth* glc5;

public slots:
	void set_vector();
	void connect_cloth();
	void disconnect_cloth();
	void update_spring();
	void draw_cloth();
	void cloth_updating();
	void fps_count();

protected:

	void paintGL() override;
	void initializeGL() override;
	void resizeGL(int width, int height) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void set_perspective(float p);
	void set_drawMode(int mode);
	void view_anim(float v);

	

	QString status;




private:

	int m_connection, count, m_collision;
	void drawing_cloth();
	void set_status(const QString& s);
	void clear_status();
	

	

	QMatrix4x4 transform_matrix() const;
	QMatrix4x4 view_matrix() const;

	QOpenGLShaderProgram mesh_shader;
	QOpenGLShaderProgram mesh_wireframe_shader;
	QOpenGLShaderProgram quad_shader;

	GLMesh* mesh;
	Backdrop* backdrop;
	

	GLConnect* con_main;

	GLConnect* con_sub1;
	GLConnect* con_sub2;

	GLConnect* con_left1;
	GLConnect* con_left2;

	GLConnect* con_right1;
	GLConnect* con_right2;

	

	int main_connect, sub_connect, left_connect, right_connect;

	QTimer* timer;

	QTimer* fps_timer;

	QVector3D center;
	float scale;
	float zoom;
	float tilt;
	float yaw;
	int m_cloth;
	float perspective;
	int drawMode;
	Q_PROPERTY(float perspective MEMBER perspective WRITE set_perspective);
	QPropertyAnimation anim;
	QPoint mouse_pos;



};



#endif // CANVAS_H
