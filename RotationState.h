#ifndef ROTATIONSTATEH
#define ROTATIONSTATEH
#include <QMatrix4x4>
#include <QTimer>
#include <QMutex>
#include <QObject>
#include "Cube.h"

class RotationState:QObject{
	Q_OBJECT
	public:
		const static int operationIndexs[9][12];
		const static int operationLateralIndexs[6][8];
		RotationState();
        ~RotationState();
        bool rotateAxisSetted;
		int controledIndex;
		double remainDegree;
		int state[54];
		QVector4D rotateAxis;
		Cube* allCube[27];
		Cube* reArrangedCubes[3][9];
		int operatingCubesIndex;
		double rotateDegrees[3];
		QVector4D objTangent;
		QVector4D refPoint;
		QMatrix4x4 rotateM;
		QMatrix4x4 backups[3][9];
		QTimer animateTimer;
		QMutex mutex;
		void rotateCubes(double degree,int index=-1);
		void changeState(int kind,int times);
	private:
		int getLateralIndex(int);
	private slots:
		void animate();
};
#endif
