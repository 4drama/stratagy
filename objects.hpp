#ifndef OBJECTS__
#define OBJECTS__

#include "geometry.hpp"
#include "orders.hpp"

class Object{
	using Point = geometry::Point;
	
private:	
	Point coordinate;
	
protected:
	Object() = delete;
	Object(Point coordinate_);

public:
	Point CoordinateGet() const;
	void CoordinateSet(Point newCoordinate);
	
};


namespace order{
	class Order;	
	enum class INFO;	
}

class Unit : public Object{
using Point = geometry::Point;
using INFO = order::INFO;
private:
	int health;
	order::Order *porder;
	double attackRange;
	double visibilityRange;
	
public:
	INFO Attack(Object *target);
	INFO Move(Point target);
	Object* FindEnemy();
	
	
	double gatAttackRange(){
		return attackRange;
	}
	
	double gatVisibilityRange(){
		return visibilityRange;
	}
};

#endif