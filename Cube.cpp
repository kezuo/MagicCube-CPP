#include "Cube.h"
#include "Face.cpp"
#include <map>
#include <GL/gl.h>
#define RED(c) ((unsigned int)c>>24)
#define GREEN(c) (((unsigned int)c>>16)&0x000000ff)
#define BLUE(c) (((unsigned int)c>>8)&0x000000ff)
#define ALPHA(c) (((unsigned int)c)&0x000000ff)
#define COLOR(a) RED(a),GREEN(a),BLUE(a),ALPHA(a)

const Face Cube::planes[6]=
{{{{-1,-1,-1},{-1,1,-1},{-1,1,1},{-1,-1,1},{-1,0,0}},0xffffffff,
{{0,29},{3,28},{6,27},{9,32},{12,31},{15,30},{18,35},{21,34},{24,33}}},
{{{1,-1,-1},{1,-1,1},{1,1,1},{1,1,-1},{1,0,0}},0x0000ffff,
{{2,9},{5,10},{8,11},{11,12},{14,13},{17,14},{20,15},{23,16},{26,17}}},
{{{-1,-1,-1},{-1,-1,1},{1,-1,1},{1,-1,-1},{0,-1,0}},0xff0000ff,
{{18,51},{19,52},{20,53},{21,48},{22,49},{23,50},{24,45},{25,46},{26,47}}},
{{{-1,1,-1},{1,1,-1},{1,1,1},{-1,1,1},{0,1,0}},0xff00ffff,
{{0,42},{1,43},{2,44},{3,39},{4,40},{5,41},{6,36},{7,37},{8,38}}},
{{{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{0,0,-1}},0x00ff00ff,
{{0,0},{1,1},{2,2},{9,3},{10,4},{11,5},{18,6},{19,7},{20,8}}},
{{{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1},{0,0,1}},0xffff00ff,
{{6,20},{7,19},{8,18},{15,23},{16,22},{17,21},{24,26},{25,25},{26,24}}}};
Cube::Cube():faces(),matrix(),glList(0),faces_initializer{
{&left,{0x111111ff,0xffffffff}},
{&right,{0x222222ff,0xffffffff}},
{&bottom,{0x333333ff,0xffffffff}},
{&top,{0x444444ff,0xffffffff}},
{&near,{0x555555ff,0xffffffff}},
{&far,{0x666666ff,-0xffffffff}}
}{
	unsigned int *memory;
	int index;
	for(auto& member:faces_initializer){
		memory=new unsigned int[2];
		index=0;
		for(int var:member.second)memory[index++]=var;
		faces.insert(std::pair<const Face*,unsigned int*>(member.first,memory));
	}

}
void Cube::genList(){
	if(glList)glDeleteLists(glList,1);
	glList=glGenLists(1);
	glNewList(glList,GL_COMPILE);
	for(auto& face:faces){
		if(face.second){
			glBegin(GL_TRIANGLE_FAN);
			glColor4ub(COLOR(face.second[0]));
			glVertex3dv(face.first->a);
			glVertex3dv(face.first->b);
			glVertex3dv(face.first->c);
			glVertex3dv(face.first->d);
			glEnd();
		}
	}
	glEndList();
}
Cube::~Cube(){
	for(auto& member:faces)
		delete[] member.second;
	glDeleteLists(glList,1);
}
