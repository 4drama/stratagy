#ifndef BASE_OBJECTS__
#define BASE_OBJECTS__

#include "geometry.hpp"
#include "order_info.hpp"

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

class Able_to_see : virtual public Object {
private:
	double visibilityRange;
public:
	double gatVisibilityRange(){
		return visibilityRange;
	}
};

class Able_to_move : virtual public Object, virtual public Able_to_see{
private:
	
public:
	order::INFO Move(geometry::Point target);
};

class Able_to_attack : virtual public Object, virtual public Able_to_see {
private:
	double attackRange;
public:
	double gatAttackRange(){
		return attackRange;
	}
	
	order::INFO Attack(Object *target);
	Object* FindEnemy();
};

/*
class Unit : public Object{
private:
	int health;
	
public:
	
};*/

#endif