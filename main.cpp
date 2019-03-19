#include "MagicWidget.h"
#include <QApplication>
#include <QGLFormat>
#include <QPushButton>
#include <QVBoxLayout>
int main(int argc,char** args){
    QApplication app(argc,args);
	QGLFormat format=QGLFormat();
	format.setVersion(2,0);//3.x can't work
	QGLFormat::setDefaultFormat(format);
	QWidget all;
	QPushButton button("button");
	QVBoxLayout vLayout;
	MagicWidget mgW;
	mgW.setMinimumSize(400,600);
	all.setLayout(&vLayout);
	vLayout.addWidget(&button);
	vLayout.addWidget(&mgW);
	all.show();
    QObject::connect(&button,&QPushButton::clicked,&mgW,&MagicWidget::reSet);
	return app.exec();
}	
