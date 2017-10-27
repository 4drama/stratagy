#ifndef BASE_OBJECTS__
#define BASE_OBJECTS__

#include "geometry.hpp"
#include "order_info.hpp"

#include <vector>
#include <algorithm>
#include <memory>
#include <map>

class ObjectsRoster;

struct ObjectAttributes{
	using Point = geometry::Point;
	
	Point 			coordinate;
	int 			health;
	ObjectsRoster 	*zone;
	double			visibilityRange;
	double			speed;
	double			attackSpeed;
	double			attackRange;
	double			attackDamage;
	int 			owner = 0;
	
	ObjectAttributes& setCoordinate(geometry::Point coordinate_);
	ObjectAttributes& setOwner(int owner_);
};

class Able_to_attack;
class Able_to_move;
class Able_to_see;
class Able_to_destroy;

class ObjectCast{
public:
	virtual Able_to_attack* 	CanAttack();
	virtual Able_to_move* 		CanMove();
	virtual Able_to_see* 		CanSee();
	virtual Able_to_destroy* 	CanDestroy();
};

class Object : virtual public ObjectCast{
	using Point = geometry::Point;	
private:	
	Point coordinate;
	int owner;
protected:
	Object() = delete;
	Object(const ObjectAttributes *attr);
	
public:
	Point CoordinateGet() const;
	void CoordinateSet(Point newCoordinate);
	
	int OwnerGet() const;
	void OwnerSet(int owner_);
	
	virtual void Tick(float time);
	virtual ~Object() = default; 
};

class ObjectsRoster{
private:
	std::vector<Object*> toErise;
	std::map<unsigned, std::unique_ptr<Object> > roster;
	
	void Erase(Object* obj);
public:

	unsigned Add(std::unique_ptr<Object> obj);
	std::unique_ptr<Object>& operator[](unsigned id);
	
	void ToErase(Object* obj);
	
	template<typename F>
	void for_each(F&& fun) const{		
		std::for_each(roster.begin(), roster.end(),[fun]
		(const std::pair<const unsigned, std::unique_ptr<Object> > &obj){
			fun(obj.second);
		});
	}
	
	void Tick(float time);
};

class Destructible_object;

class Interacts_with_other{
private:
	ObjectsRoster *zone;
public:
	Interacts_with_other() = delete;
	Interacts_with_other(const ObjectAttributes *attr);
	
	ObjectsRoster* getZone() const;
};

class Able_to_destroy : virtual public Object, virtual public Interacts_with_other{
private:
	std::shared_ptr<Destructible_object> sharedObj;
	int maxHealth;
	int currHealth;
public:
	Able_to_destroy() = delete;
	Able_to_destroy(const ObjectAttributes *attr);
	Able_to_destroy* CanDestroy() override;
	
	int GetMaxHealth() const;
	int GetCurrHealth() const;
	void Damage(int damage);
	
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

class Able_to_see : virtual public Object, virtual public Interacts_with_other{
private:
	double visibilityRange;
public:
	Able_to_see() = delete;
	Able_to_see(const ObjectAttributes *attr);
	Able_to_see* CanSee() override;
	
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
	Able_to_move* CanMove() override;
	
	double getSpeed() const;
	
	void Move(float time);
	order::INFO MoveUpdate(geometry::Point target);
};

class Able_to_attack : virtual public Object, virtual public Able_to_see {
private:
	double attackSpeed;
	double attackDelay;
	double attackRange;
	double attackDamage;
	
	std::shared_ptr<Destructible_object> actionTarget = nullptr;
public:
	Able_to_attack() = delete;
	Able_to_attack(const ObjectAttributes *attr);
	Able_to_attack* CanAttack() override;
	
	double getAttackRange() const;
	
	order::INFO Attack(float time);
	void AttackCancel();
	
	void setTarget(std::shared_ptr<Destructible_object> newTarget);
	std::shared_ptr<Destructible_object> getTarget() const;
	order::INFO AttackUpdate(std::shared_ptr<Destructible_object> target);
};

#endif