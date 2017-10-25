#include "base_objects.hpp"

#include <utility>
#include <map>
#include <algorithm>

#include <cassert>

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

void ObjectsRoster::Erase(Object* objdel){
	auto it = std::find_if(roster.begin(), roster.end(),
				[objdel](std::unique_ptr<Object> &objcurr) -> bool{
		if (objdel == objcurr.get())
			return true;
		else
			return false;
	});
	
	roster.erase(it);
}

void ObjectsRoster::ToErase(Object* obj){
	toErise.push_back(obj);
}

void ObjectsRoster::Tick(float time){
	std::for_each(roster.begin(), roster.end(),[time]
		(std::unique_ptr<Object> &obj){
			obj->Tick(time);
		});
	
	std::for_each(toErise.begin(), toErise.end(),[this]
		(Object *obj){
			this->Erase(obj);
		});
	
	this->toErise.clear();
}

Interacts_with_other::Interacts_with_other(const ObjectAttributes *attr)
		:	zone(attr->zone){
	
}

ObjectsRoster* Interacts_with_other::getZone() const{
	return zone;
}

Able_to_destroy::Able_to_destroy(const ObjectAttributes *attr)
		:	Object(attr),
			Interacts_with_other(attr),
			maxHealth(attr->health),
			currHealth(maxHealth){
	sharedObj = std::make_shared<Destructible_object>(this);
}

int Able_to_destroy::GetMaxHealth() const{
	return maxHealth;
}

int Able_to_destroy::GetCurrHealth() const{
	return currHealth;
}

void Able_to_destroy::Damage(int damage){
	currHealth -= damage;
	if(currHealth <= 0){
		this->getZone()->ToErase(this);
		return;
	}
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
		:	Object(attr),
			Interacts_with_other(attr),
			visibilityRange(attr->visibilityRange){	
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
	
	this->getZone()->for_each(procedure);
	auto nearest = all.lower_bound(0);
	if(nearest->second->getObject() == dynamic_cast<const Able_to_destroy*>(this))
		nearest++;
	std::cerr 	<< '(' << this->CoordinateGet().x << ',' << this->CoordinateGet().y << ')' 
			<< " find distance:" << nearest->first << std::endl;
	return nearest->second;
}

Able_to_move::Able_to_move(const ObjectAttributes *attr)
		:	Object(attr),
			Interacts_with_other(attr),
			Able_to_see(attr),
			speed(attr->speed){
}

double Able_to_move::getSpeed() const{
	return speed;
}

void Able_to_move::Move(float time){
	using geometry::Shift;
	
	geometry::Point newPosition;
	newPosition = Shift(this->CoordinateGet(), this->actionPosition, time/1000, this->speed);
	this->CoordinateSet(newPosition);
}

order::INFO Able_to_move::MoveUpdate(geometry::Point target){
	this->actionPosition = target;
}

Able_to_attack::Able_to_attack(const ObjectAttributes *attr)
		:	Object(attr),
			Interacts_with_other(attr),
			Able_to_see(attr),
			attackSpeed(attr->attackSpeed),
			attackDelay(0),
			attackRange(attr->attackRange),
			attackDamage(attr->attackDamage){	
}

double Able_to_attack::getAttackRange() const{
	return attackRange;
}

void Able_to_attack::Attack(float time){
	if (attackDelay >= attackSpeed){			//TODO reconsider delay
		attackDelay = 0;	
		if(actionTarget->getLive()){
			actionTarget->getObject()->Damage(attackDamage);
		} else {
			actionTarget = nullptr;
			attackDelay = attackSpeed;
			return;
		}
	} else if(actionTarget != nullptr){
		attackDelay += time;
		return;
	} else {
		
	}
}

void Able_to_attack::setTarget(std::shared_ptr<Destructible_object> newTarget){
	this->actionTarget = newTarget;
	return;
}

std::shared_ptr<Destructible_object> Able_to_attack::getTarget() const{
	assert(actionTarget != nullptr);
	return actionTarget;
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