#include "RotationState.h"
#include <iostream>
#include <cmath>
const int RotationState::operationIndexs[9][12]={
	{6,3,0,42,39,36,20,23,26,45,48,51},
	{7,4,1,43,40,37,19,22,25,46,49,52},
	{8,5,2,44,41,38,18,21,24,47,50,53},
	{8,7,6,35,34,33,26,25,24,17,16,15},
	{5,4,3,32,31,30,23,22,21,14,13,12},
	{2,1,0,29,28,27,20,19,18,11,10,9},
	{44,43,42,29,32,35,51,52,53,15,12,9},
	{41,40,39,28,31,34,48,49,50,16,13,10},
	{38,37,36,27,30,33,45,46,47,17,14,11}};
const int RotationState::operationLateralIndexs[6][8]={
	{35,32,29,28,27,30,33,34},
	{15,12,9,10,11,14,17,16},
	{53,52,51,48,45,46,47,50},
	{44,43,42,39,36,37,38,41},
	{2,1,0,3,6,7,8,5},
	{18,19,20,23,26,25,24,21}};
RotationState::RotationState():
rotateAxisSetted(false),
controledIndex(-1),
remainDegree(0),
operatingCubesIndex(-1),
rotateDegrees{0,0,0},
mutex(){
	int i;
	for(i=0;i<54;i++)state[i]=i;
	animateTimer.setSingleShot(false);
	animateTimer.setInterval(50);
	connect(&animateTimer,SIGNAL(timeout()),this,SLOT(animate()));
	animateTimer.start();
}
void RotationState::animate(){
	int i;
	int index;
	double remain;
	double fitDegree;
	if(controledIndex!=-1){
		index=controledIndex;
		remain=remainDegree;
		if(abs(remain)<=4){
			controledIndex=-1;
			remainDegree=0;
			rotateCubes(remain,index);
		}
		else if(remain>0){
			remainDegree-=4;
			rotateCubes(4,controledIndex);
		}
		else{
			remainDegree+=4;
			rotateCubes(-4,controledIndex);
		}
	}
	for(i=0;i<3;i++)if(operatingCubesIndex!=i&&rotateDegrees[i]){
		fitDegree=round(rotateDegrees[i]/90)*90;
		if(abs(rotateDegrees[i]-fitDegree)<2)rotateCubes(fitDegree-rotateDegrees[i],i);
		else if(fitDegree>rotateDegrees[i])rotateCubes(2,i);
		else rotateCubes(-2,i);
	}
}
void RotationState::rotateCubes(double degree,int index){
	mutex.lock();
	int i;
	int times;
	int uniformIndex;
	double fitDegree;
	int kind;
	if(!rotateAxisSetted){
		mutex.unlock();
		return;
	}
	if(index==-1)index=operatingCubesIndex;
	if(rotateDegrees[index]==0)
		for(i=0;i<9;i++)backups[index][i]=reArrangedCubes[index][i]->matrix;
	rotateM.setToIdentity();
	rotateDegrees[index]+=degree;
	fitDegree=round(rotateDegrees[index]/90.0)*90;
	if(abs(rotateDegrees[index]-fitDegree)<0.000001){
		rotateDegrees[index]=0;
		rotateM.rotate(fitDegree,rotateAxis.x(),rotateAxis.y(),rotateAxis.z());
		times=round(fitDegree/90);
		uniformIndex=index;
		if(rotateAxis.x()){
			if(rotateAxis.x()<0){
				times*=-1;
				uniformIndex=2-index;
			}
			kind=uniformIndex;
		}
		else if(rotateAxis.y()){
			if(rotateAxis.y()<0){
				times*=-1;
				uniformIndex=2-index;
			}
			kind=uniformIndex+3;
		}
		else if(rotateAxis.z()){
			if(rotateAxis.z()<0){
				times*=-1;
				uniformIndex=2-index;
			}
			kind=uniformIndex+6;
		}
		if(!remainDegree&&!rotateDegrees[0]&&!rotateDegrees[1]&&!rotateDegrees[2])rotateAxisSetted=false;
		for(i=0;i<9;i++)reArrangedCubes[index][i]->matrix=rotateM*backups[index][i];
		changeState(kind,int(times));
	}
	else{
		rotateM.rotate(degree,rotateAxis.x(),rotateAxis.y(),rotateAxis.z());
		for(auto cube:reArrangedCubes[index])cube->matrix=rotateM*(cube->matrix);
	}
	mutex.unlock();
}
int RotationState::getLateralIndex(int kind){
	if(kind==0)return 0;
	if(kind==2)return 1;
	if(kind==3)return 2;
	if(kind==5)return 3;
	if(kind==6)return 4;
	if(kind==8)return 5;
	return -1;
}
void RotationState::changeState(int kind,int times){
	times%=4;
	std::cout<<"changestate "<<kind<<" "<<times<<endl;
	for(int i=0;i<times;i++){
		int a,b,c;
		a=state[operationIndexs[kind][9]];
		b=state[operationIndexs[kind][10]];
		b=state[operationIndexs[kind][11]];
		for(int j=2;j>=0;j--){
			state[operationIndexs[kind][(j+1)*3]]=state[operationIndexs[kind][j*3]];
			state[operationIndexs[kind][(j+1)*3+1]]=state[operationIndexs[kind][j*3+1]];
			state[operationIndexs[kind][(j+1)*3+2]]=state[operationIndexs[kind][j*3+2]];
		}
		state[operationIndexs[kind][0]]=a;
		state[operationIndexs[kind][1]]=b;
		state[operationIndexs[kind][2]]=c;
	}
	if(!(kind==1||kind==4||kind==7))
	for(int i=0;i<times;i++){
		int a=state[operationLateralIndexs[getLateralIndex(kind)][6]];
		int b=state[operationLateralIndexs[getLateralIndex(kind)][7]];
		for(int j=2;j>=0;j--){
			state[operationLateralIndexs[getLateralIndex(kind)][(j+1)*2]]=state[operationLateralIndexs[getLateralIndex(kind)][j*2]];
			state[operationLateralIndexs[getLateralIndex(kind)][(j+1)*2+1]]=state[operationLateralIndexs[getLateralIndex(kind)][j*2+1]];
		}
		state[operationLateralIndexs[getLateralIndex(kind)][0]]=a;
		state[operationLateralIndexs[getLateralIndex(kind)][1]]=b;
	}
/*	current=owner.currentState()
	if current==False:
		print 'unaligned'
		return
	for i in range(54):
		if state[i]!=current[i]:
			print 'state error'
			return
	print 'ok'*/
}	
RotationState::~RotationState(){
}
