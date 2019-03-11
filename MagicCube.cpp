#include "MagicCube.h"
#include <GL/gl.h>

MagicCube::MagicCube(){
	for(auto& plane:Cube::planes){
		for(int i=0;i<9;i++){
			cubes[plane.indexs[i][0]].faces[&plane][0]=plane.color;
			cubes[plane.indexs[i][0]].faces[&plane][1]=plane.indexs[i][1];
			cubes[plane.indexs[i][0]].matrix.translate(plane.normal[0],plane.normal[1],plane.normal[2]);
			QVector3D planePosition=QVector3D(plane.normal[0],plane.normal[1],plane.normal[2]);
			centerPosition[plane.indexs[i][1]]=cubes[plane.indexs[i][0]].matrix*planePosition;
		}
	}
}
void MagicCube::genLists(){
	for(auto& cube:cubes)cube.genList();
}
int* MagicCube::currentState(){
}
bool MagicCube::operate(int kind,int times){
}
void MagicCube::draw(){
	for(auto& cube:cubes){
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(cube.matrix.data());
		glCallList(cube.glList);
		glPopMatrix();
	}
}

bool MagicCube::operaBegin(int wx,int wy){
	return false;
}
bool MagicCube::operaContin(int wx,int wy){
	return false;
}

bool MagicCube::operaEnd(int wx,int wy){
	return false;
}
