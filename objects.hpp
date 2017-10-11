#ifndef OBJECTS__
#define OBJECTS__

#include "geometry.hpp"

class Object{
	using geometry::Point Point;
	
private:	
	Point coordinate;
	
protected:
	Object() = delete;
	Object(Point coordinate_);

public:
	Point CoordinateGet() const;
	void CoordinateSet(Point newCoordinate);
	
};

#include "orders.hpp"

class Order;
enum class INFO;

class Unit : public Object{
using order::Order Order;
private:
	int health;
	Order *porder;
	double attackRange;
	
public:
	INFO Attack(Object *target);
	INFO Move(Point target);
	
	double gatRange(){
		return attackRange;
	}
}

#endif