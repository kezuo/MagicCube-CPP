#ifndef CUBEH
#define CUBEH
#include "Face.cpp"
#include <map>
#include <QMatrix4x4>

using namespace std;
class Cube{
	public:
		static const Face planes[6];
		const Face &left=planes[0];
		const Face &right=planes[1];
        const Face &nea=planes[2];
        const Face &fa=planes[3];
		const Face &bottom=planes[4];
		const Face &top=planes[5];
		initializer_list<pair<const Face*,initializer_list<unsigned int>>> faces_initializer;
		map<const Face*,unsigned int*> faces;
		QMatrix4x4 matrix;
		int glList;
		Cube();
		void genList();
		~Cube();
};		
#endif
