#ifndef GEOMETRY__
#define GEOMETRY__

namespace geometry{
	
	struct Point{
		double x;
		double y;
	};
	
	struct Difference{
		double x;
		double y;
	};
	
	Difference GetDifference(Point first, Point second);
	
	const bool operator==(const Point &arg1, const Point &arg2);
	
	const Difference operator*(const Difference &arg1, double arg2);
	
	const Point operator+(const Point &arg1, const Difference &arg2);
	
	double Range(Point first, Point second);
	
	Point Shift(Point start_point, Point end_point, double time, double speed);
}

#endif