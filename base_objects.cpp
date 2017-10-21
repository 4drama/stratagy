#include "base_objects.hpp"

#include <utility>
#include <map>

#include <iostream>

ObjectAttributes* ObjectAttributes::setCoordinate(geometry::Point coordinate_){
	coordinate = coordinate_;
	return this;
}

Object::Object(const ObjectAttributes *attr)
		:	coordinate(attr->coordinate){	
}

geometry::Point Object::CoordinateGet() const{
	return coordinate;
}

void Object::Tick(float time){
}

void ObjectsRoster::Add(std::unique_ptr<Object> obj){
	roster.push_back(std::move(obj));
}

void ObjectsRoster::Tick(float time){
	std::for_each(roster.begin(), roster.end(),[time]
		(std::unique_ptr<Object> &obj){
			obj->Tick(time);
		});
};

Able_to_destroy::Able_to_destroy(const ObjectAttributes *attr)
		:	Object(attr){
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

Able_to_see::Able_to_see(const ObjectAttributes *attr)
		:	Object(attr), zone(attr->zone), visibilityRange(attr->visibilityRange){	
}

double Able_to_see::getVisibilityRange() const{
	return visibilityRange;
}

std::shared_ptr<Destructible_object> Able_to_see::FindEnemy() const{
	std::map<double, std::shared_ptr<Destructible_object> > all;
	//TO DO : NEED DIFFER ALLIED UNITS
	auto procedure = [&all, this](const std::unique_ptr<Object> &other){
		if(Able_to_destroy *current = dynamic_cast<Able_to_destroy*>(other.get())){
			double range = geometry::Range(this->CoordinateGet(), current->CoordinateGet());
			std::shared_ptr<Destructible_object> ptr = current->getSharedObject();
			all[range] = ptr;
			return;
		} else
			return;
	};	
	
	zone->for_each(procedure);
	auto nearest = all.lower_bound(0);
	if(nearest->second->getObject() == dynamic_cast<const Able_to_destroy*>(this))
		nearest++;
	std::cerr 	<< '(' << this->CoordinateGet().x << ',' << this->CoordinateGet().y << ')' 
			<< " find distance:" << nearest->first << std::endl;
	return nearest->second;
}

Able_to_move::Able_to_move(const ObjectAttributes *attr)
		:	Object(attr),
			Able_to_see(attr),
			speed(attr->speed){
}

double Able_to_move::getSpeed() const{
	return speed;
}

order::INFO Able_to_move::MoveUpdate(geometry::Point target){
	//TO DO
}

Able_to_attack::Able_to_attack(const ObjectAttributes *attr)
		:	Object(attr),
			Able_to_see(attr),
			attackRange(attr->attackRange){	
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