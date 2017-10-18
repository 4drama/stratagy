#ifndef BASE_OBJECTS__
#define BASE_OBJECTS__

#include "geometry.hpp"
#include "order_info.hpp"

#include <vector>
#include <algorithm>
#include <memory>
#include <list>
//#include <functional>

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
	virtual ~Object() = default;
};

class ObjectsRoster{
private:
	std::vector<std::unique_ptr<Object> > roster;
public:

	void Add(Object&& obj);
	
	};
};

class Destructible_object;

class Able_to_destroy : virtual public Object {
private:
	std::shared_ptr<Destructible_object> sharedObj;
	int health;
public:
	Able_to_destroy() = delete;
	Able_to_destroy(geometry::Point coordinate_);
	
	std::shared_ptr<Destructible_object> getSharedObject() const;
	~Able_to_destroy();
};

class Destructible_object{
private:
	friend class Able_to_destroy;
	Able_to_destroy *object;
public:
	Destructible_object() = delete;
	Destructible_object(Able_to_destroy* object_);
	
	bool getLive() const;
	Able_to_destroy* getObject() const;
};

class Able_to_see : virtual public Object {
private:
	ObjectsRoster *zone;
	double visibilityRange;
public:
	Able_to_see() = delete;
	Able_to_see(ObjectsRoster *zone_, double visibilityRange_, geometry::Point coordinate_);
	
	double getVisibilityRange() const;
	
	std::shared_ptr<Destructible_object> FindEnemy() const;
};

class Able_to_move : virtual public Object, virtual public Able_to_see{
private:
	double speed;
	geometry::Point actionPosition;
public:
	Able_to_move() = delete;
	Able_to_move(double speed_,ObjectsRoster *zone_, double visibilityRange_, geometry::Point coordinate_);
	
	double getSpeed() const;
	
	order::INFO MoveUpdate(geometry::Point target);
};

class Able_to_attack : virtual public Object, virtual public Able_to_see {
private:
	double attackRange;
	std::shared_ptr<Destructible_object> actionTarget;
public:
	Able_to_attack() = delete;
	Able_to_attack(double attackRange_, ObjectsRoster *zone_, double visibilityRange_, geometry::Point coordinate_);
	
	double getAttackRange() const;
	
	order::INFO AttackUpdate(std::shared_ptr<Destructible_object> target);
};

#endif