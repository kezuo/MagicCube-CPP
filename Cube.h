#ifndef CUBEH
#define CUBEH
#include "Face.cpp"
#include <map>
#include <QMatrix4x4>

using namespace std;
class Cube{
	public:
		static const Face planes[6];
        static const Face &left;
        static const Face &right;
        static const Face &nea;
        static const Face &fa;
        static const Face &bottom;
        static const Face &top;
		initializer_list<pair<const Face*,initializer_list<unsigned int>>> faces_initializer;
		map<const Face*,unsigned int*> faces;
		QMatrix4x4 matrix;
		int glList;
		Cube();
		void genList();
		~Cube();
};		
#endif
