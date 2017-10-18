#include "geometry.hpp"
	
const bool geometry::operator==(const Point &arg1, const Point &arg2){
	if((arg1.x == arg2.x) && (arg1.y == arg2.y))
		return true;
	else
		return false;
}

double geometry::Range(Point first, Point second){
	
};