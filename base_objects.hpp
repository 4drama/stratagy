#ifndef BASE_OBJECTS__
#define BASE_OBJECTS__

#include "geometry.hpp"
#include "order_info.hpp"

#include <vector>
#include <algorithm>
#include <memory>

class ObjectsRoster;

struct ObjectAttributes{
	geometry::Point coordinate;
	int health;
	ObjectsRoster *zone;
	double visibilityRange;
	double speed;
	double attackRange;
	
	ObjectAttributes* setCoordinate(geometry::Point coordinate_);
};



class Object{
	using Point = geometry::Point;	
private:	
	Point coordinate;
protected:
	Object() = delete;
	Object(const ObjectAttributes *attr);
	
public:
	Point CoordinateGet() const;
	void CoordinateSet(Point newCoordinate);
	
	virtual void Tick(float time);
	virtual ~Object() = default; 
};

class ObjectsRoster{
private:
	std::vector<std::unique_ptr<Object> > roster;
public:

	void Add(std::unique_ptr<Object> obj);
	
	
	template<typename F>
	void for_each(F&& fun) const{		
		std::for_each(roster.begin(), roster.end(),[fun]
		(const std::unique_ptr<Object> &obj){
			fun(obj);
		});
	}
	
	void Tick(float time);
};

class Destructible_object;

class Able_to_destroy : virtual public Object {
private:
	std::shared_ptr<Destructible_object> sharedObj;
	int health;
public:
	Able_to_destroy() = delete;
	Able_to_destroy(const ObjectAttributes *attr);
	
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
	Able_to_see(const ObjectAttributes *attr);
	
	double getVisibilityRange() const;
	
	std::shared_ptr<Destructible_object> FindEnemy() const;
};

class Able_to_move : virtual public Object, virtual public Able_to_see{
private:
	double speed;
	geometry::Point actionPosition;
public:
	Able_to_move() = delete;
	Able_to_move(const ObjectAttributes *attr);
	
	double getSpeed() const;
	
	void Move(float time){};
	order::INFO MoveUpdate(geometry::Point target);
};

class Able_to_attack : virtual public Object, virtual public Able_to_see {
private:
	double attackSpeed;
	double attackRange;
	double attackDamage;
	
	std::shared_ptr<Destructible_object> actionTarget;
public:
	Able_to_attack() = delete;
	Able_to_attack(const ObjectAttributes *attr);
	
	double getAttackRange() const;
	
	void Attack(float time){};
	void setTarget(std::shared_ptr<Destructible_object> newTarget){};
	std::shared_ptr<Destructible_object> getTarget(){};
	order::INFO AttackUpdate(std::shared_ptr<Destructible_object> target);
};

#endif