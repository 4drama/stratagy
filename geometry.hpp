#ifndef GEOMETRY__
#define GEOMETRY__

namespace geometry{
	
	struct Point{
		double x;
		double y;
	};
	
	const bool operator==(const Point &arg1, const Point &arg2){
		if((arg1.x == arg2.x) && (arg1.y == arg2.y))
			return true;
		else
			return false;
	}
	
	double Range(Point first, Point second);
	
}

#endif