#ifndef MAGICCUBEH
#define MAGICCUBEH
#include <QVector4D>
#include "Cube.h"
#include "Face.cpp"
#include "RotationState.h"
class MagicCube{
	public:
		MagicCube();
		RotationState rotationState;
		int* currentState();
		bool operate(int kind,int times);
		void draw();
		void genLists();
		bool operaBegin(int wx,int wy);
		bool operaContin(int wx,int wy);
		bool operaEnd(int wx,int wy);
		int side;
		int centerX,centerY;
	private:
		int curState[54];
		Cube cubes[27];
		QVector4D centerPosition[54];
		double projection[16];
		double model[16];
		int viewport[4];
		int oldX;
		int oldY;
		double x,y,z,nSx,nSy,nSz;
		const Cube* curCube;
		const Face* curFace;
};
#endif
