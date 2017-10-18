#ifndef GEOMETRY__
#define GEOMETRY__

namespace geometry{
	
	struct Point{
		double x;
		double y;
	};
	
	const bool operator==(const Point &arg1, const Point &arg2);
	
	double Range(Point first, Point second);
	
}

#endif