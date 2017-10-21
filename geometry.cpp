#include "geometry.hpp"

#include <cmath>

const bool geometry::operator==(const Point &arg1, const Point &arg2){
	if((arg1.x == arg2.x) && (arg1.y == arg2.y))
		return true;
	else
		return false;
}

double geometry::Range(Point first, Point second){
	return std::sqrt( std::pow((second.x - first.x),2) + std::pow((second.y - first.y),2));
};