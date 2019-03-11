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
	private:
		int curState[54];
		Cube cubes[27];
		QVector4D centerPosition[54];
		int oldX;
		int oldY;
		Cube* curCube;
		Face* curFace;
};
#endif
