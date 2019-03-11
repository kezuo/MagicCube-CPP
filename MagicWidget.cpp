#include "MagicWidget.h"
#include <GL/gl.h>
#include <QPainter>
#include <cmath>

MagicWidget::MagicWidget(QWidget* parent):QGLWidget(parent){
	setAutoFillBackground(false);
	modelView.scale(0.125,0.125,0.125);
	timer.setSingleShot(false);
	connect(&timer,SIGNAL(timeout()),this,SLOT(animate()));
	timer.setInterval(25);
	timer.start();
}

void MagicWidget::initializeGL(){
	passEvent=false;
	magicCube.genLists();
}
void MagicWidget::animate(){
	update();
}
void MagicWidget::setGL(){
	glEnable(GL_BLEND);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
	glPointSize(3);
	glClearColor(0,0,0,0);
	glClearDepth(1);
	glDepthRange(0,1);
        glViewport((w - side) / 2, (h - side) / 2, side, side);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelView.data());
}
void MagicWidget::paintGL(){
	setGL();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	magicCube.draw();
}
void MagicWidget::resizeGL(int w,int h){
	this->w=w;
	this->h=h;
	side=w<h?w:h;
	centerX=w/2;
	centerY=h/2;
	magicCube.centerX=centerX;
	magicCube.centerY=centerY;
	magicCube.side=side;
}
void MagicWidget::mousePressEvent(QMouseEvent* event){
	oldX=event->x();
	oldY=h-event->y();
	rotateM.setToIdentity();
	if(!passEvent){
		passEvent=magicCube.operaBegin(oldX,oldY);
	}
}
void MagicWidget::mouseMoveEvent(QMouseEvent* event){
	int curX=event->x();
	int curY=h-event->y();
	if(passEvent){
		magicCube.operaContin(curX,curY);
		return;
	}
	int dX=curX-oldX;
	int dY=curY-oldY;
	int axisX=dY;
	int axisY=-dX;
	double degree=sqrt(dX*dX+dY*dY)*360/side;
	rotateM.setToIdentity();
	rotateM.rotate(degree,axisX,axisY,0);
	modelView=rotateM*modelView;	
	oldX=curX;
	oldY=curY;
}
void MagicWidget::mouseReleaseEvent(QMouseEvent* event){
	int endX=event->x();
	int endY=h-event->y();
	if(passEvent){
		passEvent=false;
		magicCube.operaEnd(endX,endY);
	}
}
