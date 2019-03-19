#include "MagicCube.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <map>
#include <list>
#include <cmath>
#define GL_UNSIGNED_INT_8_8_8_8 0x8035

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
	for(int i=0;i<54;i++)
		for(auto& cube:cubes)
			for(auto pair:cube.faces){
				QVector4D sub=cube.matrix*QVector4D((float)(pair.first->normal[0]),(float)(pair.first->normal[1]),(float)(pair.first->normal[2]),1)-centerPosition[i];
				if(sub.length()<0.01)curState[i]=pair.second[1];
			}
	return curState;
}

bool MagicCube::operate(int kind,int times){
	rotationState.mutex.lock();
	if(kind<0||kind>8||rotationState.remainDegree!=0||rotationState.rotateDegrees[0]!=0||rotationState.rotateDegrees[1]!=0||rotationState.rotateDegrees[2]!=0){
		rotationState.mutex.unlock();
		return false;
	}
	int direction=kind/3;
	rotationState.controledIndex=kind%3;
	rotationState.rotateAxis=QVector4D(0,0,0,1);
	rotationState.rotateAxis[direction]=1;
	rotationState.remainDegree=times*90;
	QVector4D refPoint=QVector4D(0,0,0,1);
	QVector4D origin=refPoint;
	map<double,list<Cube*>> mapToCubes={};
	for(auto& cube:cubes){
		QVector4D originTranslated=cube.matrix*origin;
		double normalComponent=QVector4D::dotProduct(originTranslated-refPoint,rotationState.rotateAxis)/rotationState.rotateAxis.length();
		if(mapToCubes.find(normalComponent)!=mapToCubes.end())mapToCubes[normalComponent].push_back(&cube);
		else mapToCubes[normalComponent]=list<Cube*>{&cube};
	}
	int i=0;
	for(auto& pair:mapToCubes)
		for(auto& cube:pair.second){
			rotationState.reArrangedCubes[i/9][i%9]=cube;
			i++;
		}
	rotationState.mutex.unlock();
	return true;
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
	oldX=wx;
	oldY=wy;
	float depth;
	glReadPixels(wx,wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	unsigned int color;
	glReadPixels(wx,wy,1,1,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,&color);
	bool finded=false;
 	glGetDoublev(GL_PROJECTION_MATRIX, projection);  
 	glGetIntegerv(GL_VIEWPORT, viewport);  	
	for(auto& curCube:cubes){
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(curCube.matrix.data());
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glPopMatrix();
		gluUnProject(wx,wy,depth,model,projection,viewport,&nSx,&nSy,&nSz);
		if(abs(nSx)<1.03&&abs(nSy)<1.03&&abs(nSz)<1.03){
			finded=true;
			this->curCube=&curCube;
			break;
		}
	}
	if(!finded)return false;
	for(auto& pair:curCube->faces)
		if(pair.second[0]==color){
			curFace=pair.first;
			break;
		}
	return true;
}
bool MagicCube::operaContin(int wx,int wy){
	int i=0;
	bool finded;
	if(!rotationState.rotateAxisSetted){
		float depth;
		glReadPixels(wx,wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		unsigned int color;
		glReadPixels(wx,wy,1,1,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,&color);
 		glGetDoublev(GL_PROJECTION_MATRIX, projection); 
 		glGetIntegerv(GL_VIEWPORT, viewport);	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(curCube->matrix.data());
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glPopMatrix();
		gluUnProject(wx,wy,depth,model,projection,viewport,&x,&y,&z);
		if(abs(x)>1.03||abs(y)>1.03||abs(z)>1.03)return false;
		QVector3D dV(x-nSx,y-nSy,z-nSz);
		if(dV.length()<0.02)return false;
		list<int> tmp{0,1,2};
		for(auto outAxisIndex:tmp)
			if(curFace->normal[outAxisIndex]){
				tmp.remove(outAxisIndex);
				dV[outAxisIndex]=0;
				break;
			}
		list<int>::iterator p=tmp.begin();
		int indexs[2];
		indexs[0]=*p;
		indexs[1]=*(++p);
		int alongAxisIndex;
		if(abs(dV[indexs[0]])>abs(dV[indexs[1]])){
			dV[indexs[1]]=0;
			alongAxisIndex=indexs[0];
		}
		else{
			dV[indexs[0]]=0;
			alongAxisIndex=indexs[1];
		}
		QVector3D origin(0,0,0);
		QVector3D rotateEndPoint=curCube->matrix*QVector3D::crossProduct(QVector3D(curFace->normal[0],curFace->normal[1],curFace->normal[2]),dV);
		QVector3D refPoint=curCube->matrix*origin;
		rotationState.objTangent=curCube->matrix*dV-refPoint;	
		rotationState.rotateAxis=rotateEndPoint-refPoint;
		rotationState.rotateAxisSetted=true;
		map<double,list<Cube*>> mapToCubes={};
		list<double> keys;
		for(auto& cube:cubes){
			QVector4D originTranslated=cube.matrix*origin;
			double normalComponent=QVector4D::dotProduct(originTranslated-refPoint,rotationState.rotateAxis)/rotationState.rotateAxis.length();
			if(mapToCubes.find(normalComponent)!=mapToCubes.end())mapToCubes[normalComponent].push_back(&cube);
			else{
				mapToCubes[normalComponent]=list<Cube*>{&cube};
				keys.push_back(normalComponent);
			}
			if(abs(normalComponent)<Face::dist/3000)i++;
		}
		if(i!=9){
			rotationState.rotateAxisSetted=false;
			return false;
		}
		keys.sort();
		i=0;
		for(auto normalComponent:keys)
			for(auto& cube:mapToCubes[normalComponent]){
				rotationState.reArrangedCubes[i/9][i%9]=cube;
				if(abs(normalComponent)<Face::dist/3000&&rotationState.operatingCubesIndex==-1)rotationState.operatingCubesIndex=i/9;
				i++;
			}
	}
	else if(rotationState.operatingCubesIndex==-1){
		finded=false;
		for(i=0;i<3;i++){
            for(auto& cube:rotationState.reArrangedCubes[i])if(cube==curCube){
				finded=true;
				break;
			}
			if(finded)break;
		}
        if(finded)rotationState.operatingCubesIndex=i;
        else return false;
	}
	else{
 		glGetDoublev(GL_PROJECTION_MATRIX, projection); 
 		glGetIntegerv(GL_VIEWPORT, viewport);	
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		double a,b,c;
		gluProject(rotationState.objTangent[0],rotationState.objTangent[1],rotationState.objTangent[2],model,projection,viewport,&a,&b,&c);
		QVector3D winTan(a-centerX,b-centerY,0);
		double degree=QVector3D::dotProduct(QVector3D(wx-oldX,wy-oldY,0),winTan)*360/winTan.length()/side;
		rotationState.rotateCubes(degree);
	}
        oldX=wx;
        oldY=wy;
    return true;
}

bool MagicCube::operaEnd(int wx,int wy){
	rotationState.operatingCubesIndex=-1;
	return false;
}
