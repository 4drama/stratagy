#include "base_objects.hpp"

Object::Object(Point coordinate_)
		:	coordinate(coordinate_){	
}

geometry::Point Object::CoordinateGet() const{
	return coordinate;
}

Able_to_destroy::Able_to_destroy(geometry::Point coordinate_)
		:	Object(coordinate_){	
	sharedObj = std::make_shared<Destructible_object>(this);
}

std::shared_ptr<Destructible_object> Able_to_destroy::getSharedObject() const{
	return sharedObj;
}

Able_to_destroy::~Able_to_destroy(){
	sharedObj->object = nullptr;
}

Destructible_object::Destructible_object(Able_to_destroy* object_)
		:	object(object_){	
}

bool Destructible_object::getLive() const{
	return object != nullptr;
}

Able_to_destroy* Destructible_object::getObject() const{
	return object;
}

void Object::CoordinateSet(Point newCoordinate){
	coordinate = newCoordinate;
}

Able_to_see::Able_to_see(double visibilityRange_, geometry::Point coordinate_)
		:	Object(coordinate_), visibilityRange(visibilityRange_){	
}

double Able_to_see::getVisibilityRange() const{
	return visibilityRange;
}

std::shared_ptr<Destructible_object> Able_to_see::FindEnemy() const{
	//TO DO
}

Able_to_move::Able_to_move(double speed_, double visibilityRange_, geometry::Point coordinate_)
		:	Object(coordinate_),
			Able_to_see(visibilityRange_, coordinate_),
			speed(speed_){
}

double Able_to_move::getSpeed() const{
	return speed;
}

order::INFO Able_to_move::MoveUpdate(geometry::Point target){
	//TO DO
}

Able_to_attack::Able_to_attack(double attackRange_, double visibilityRange_, geometry::Point coordinate_)
		:	Object(coordinate_),
			Able_to_see(visibilityRange_, coordinate_),
			attackRange(attackRange_){	
}

double Able_to_attack::getAttackRange() const{
	return attackRange;
}

order::INFO Able_to_attack::AttackUpdate(std::shared_ptr<Destructible_object> target){
	if(target->getLive() == false)
		return order::INFO::IS_DONE;
	else if(true/*<- TO DO function check possibility*/ == false)
		return order::INFO::IMPOSSIBLY;
	else {
		actionTarget = target;
		return order::INFO::EXERCISE;
	}
}