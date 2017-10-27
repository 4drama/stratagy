#include "base_objects.hpp"

#include <utility>
#include <map>
#include <algorithm>

#include <cassert>

#include <iostream>

ObjectAttributes& ObjectAttributes::setCoordinate(geometry::Point coordinate_){
	coordinate = coordinate_;
	return *this;
}

ObjectAttributes& ObjectAttributes::setOwner(int owner_){
	owner = owner_;
	return *this;
}

Object::Object(const ObjectAttributes *attr)
		:	coordinate(attr->coordinate),
			owner(attr->owner){	
}

geometry::Point Object::CoordinateGet() const{
	return coordinate;
}

int Object::OwnerGet() const{
	return this->owner;
}

void Object::OwnerSet(int owner_){
	this->owner = owner_;
}

void Object::Tick(float time){
}

unsigned ObjectsRoster::Add(std::unique_ptr<Object> obj){
	static unsigned id = 0;
	roster.emplace(id, std::move(obj));
	return id++;
}

std::unique_ptr<Object>& ObjectsRoster::operator[](unsigned id){
	return roster[id];
}

void ObjectsRoster::Erase(Object* objdel){
	auto it = std::find_if(roster.begin(), roster.end(),
				[objdel](auto &objcurr) -> bool{
		if (objdel == objcurr.second.get())
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
		(auto &obj){
			obj.second->Tick(time);
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
	int thisOwner = this->OwnerGet();
	//TO DO : NEED DIFFER ALLIED UNITS
	auto procedure = [&all, this, thisOwner](const std::unique_ptr<Object> &other){
		Able_to_destroy *current = other.get()->CanDestroy();
		int otherOwner = other->OwnerGet();
		if(		(current != nullptr) 
			&&	(thisOwner != otherOwner) 
			&&	(otherOwner != 0)	){
			double range = geometry::Range(this->CoordinateGet(), current->CoordinateGet());
			std::shared_ptr<Destructible_object> ptr = current->getSharedObject();
			all[range] = ptr;
			return;
		} else
			return;
	};
	

	this->getZone()->for_each(procedure);
	auto nearest = all.lower_bound(0);
	
	if(all.empty())
		return nullptr;
	
	if(nearest->second->getObject() == dynamic_cast<const Able_to_destroy*>(this))
		nearest++;
	std::cerr 	<< '(' << this->CoordinateGet().x << ',' << this->CoordinateGet().y << ')' 
			<< " find " << all.size() << " distance:" << nearest->first << std::endl;
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
			attackSpeed(attr->attackSpeed * 1000),
			attackDelay(0),
			attackRange(attr->attackRange),
			attackDamage(attr->attackDamage){	
}

double Able_to_attack::getAttackRange() const{
	return attackRange;
}

order::INFO Able_to_attack::Attack(float time){
	if (attackDelay >= attackSpeed){			//TODO reconsider delay
		attackDelay = 0;	
		if(actionTarget->getLive()){
			actionTarget->getObject()->Damage(attackDamage);
			return order::INFO::IS_DONE;
		} else {
			this->AttackCancel();
			this->actionTarget = nullptr;
			return order::INFO::IMPOSSIBLY;
		}
	} else if(actionTarget != nullptr){
		attackDelay += time;
		return order::INFO::EXERCISE;
	} else {
		
	}
}

void Able_to_attack::AttackCancel(){
	this->attackDelay = 0;
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

Able_to_attack* ObjectCast::CanAttack(){
	return nullptr;
}

Able_to_move* ObjectCast::CanMove(){
	return nullptr;
}

Able_to_see* ObjectCast::CanSee(){
	return nullptr;
}

Able_to_destroy* ObjectCast::CanDestroy(){
	return nullptr;
}

Able_to_attack* Able_to_attack::CanAttack(){
	return this;
}

Able_to_move* Able_to_move::CanMove(){
	return this;
}

Able_to_see* Able_to_see::CanSee(){
	return this;
}

Able_to_destroy* Able_to_destroy::CanDestroy(){
	return this;
}