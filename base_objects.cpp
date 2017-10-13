#include "base_objects.hpp"

Object::Object(Point coordinate_)
		: coordinate(coordinate_){	
}

Point Object::CoordinateGet() const{
	return coordinate;
}

void Object::CoordinateSet(Point newCoordinate){
	coordinate = newCoordinate;
}

