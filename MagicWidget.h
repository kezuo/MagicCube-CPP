#ifndef MAGICWIDGETH
#define MAGICWIDGETH
#include <QGLWidget>
#include <QTimer>
#include "MagicCube.h"
#include <QMatrix4x4>
#include <QMouseEvent>
class MagicWidget:public QGLWidget{
	Q_OBJECT
	public:
		explicit MagicWidget(QWidget* parent=NULL);
	private:
		QTimer timer;
		bool passEvent;
		int w;
		int h;
		int centerX;
		int centerY;
		void setGL();
		int oldX;
		int oldY;
		int side;
		QMatrix4x4 rotateM;
		QMatrix4x4 modelView;
		MagicCube magicCube;
	private slots:
		void animate();
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int w,int h);
		void mousePressEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);
};	
#endif
