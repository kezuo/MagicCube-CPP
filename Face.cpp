#ifndef FACECPP
#define FACECPP
#include <initializer_list>
class Face{
	public:
		constexpr const static double dist=2.2;
		double vectors[5][3];
		double (&a)[3]=vectors[0];
		double (&b)[3]=vectors[1];
		double (&c)[3]=vectors[2];
		double (&d)[3]=vectors[3];
		double (&normal)[3]=vectors[4];
		unsigned int color;
		int indexs[9][2];
		typedef std::initializer_list<std::initializer_list<double>> double_5x3;
		typedef std::initializer_list<std::initializer_list<int>> int_9x2;
		Face(double_5x3 vectors,unsigned int color,int_9x2 indexs){
			int j,i;
			j=0;
			for(auto vector:vectors){
				i=0;
				for(auto var:vector)this->vectors[j][i++]=j!=4?var:var*dist;
				j++;
			}
			this->color=color;
			j=0;
			for(auto pair:indexs){
				i=0;
				for(auto var:pair)this->indexs[j][i++]=var;
				j++;
			}
			
		}
	private:
};
#endif
