#include "geometry.hpp"

#include <cmath>

geometry::Difference geometry::GetDifference(Point first, Point second){
	return Difference{(second.x - first.x), (second.y - first.y)};
}

const bool geometry::operator==(const Point &arg1, const Point &arg2){
	if((arg1.x == arg2.x) && (arg1.y == arg2.y))
		return true;
	else
		return false;
}

double geometry::Range(Point first, Point second){
	return std::sqrt( std::pow((second.x - first.x),2) + std::pow((second.y - first.y),2));
}

const geometry::Difference geometry::operator*(const Difference &arg1, double arg2){
	Difference res;
	res.x = arg1.x * arg2;
	res.y = arg1.y * arg2;
	return res;
}

const geometry::Point geometry::operator+(const Point &arg1, const Difference &arg2){
	Point res;
	res.x = arg1.x + arg2.x;
	res.y = arg1.y + arg2.y;
	return res;
}

geometry::Point geometry::Shift(	geometry::Point start_point,
									geometry::Point end_point,
									double 			time, 
									double 			speed){
	geometry::Difference dif = GetDifference(start_point, end_point);
	double possibleDistance = time * speed;
	double neddedDistance = Range(start_point, end_point);
	
	if(neddedDistance == 0)
		return end_point;
	
	double ratio = possibleDistance / neddedDistance;
	
	if(ratio >= 1){
		return end_point;
	} else {
		geometry::Difference traveled = dif * ratio;
		return start_point + traveled;
	}
}