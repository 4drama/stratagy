#include "orders.hpp"

namespace order{
	
	Attack::Attack(const Attributes&& entry)
				: target(entry.target){
	}
	
	order::INFO Attack::Do(Unit *current){
		double distance = geometry::Range(	current->CoordinateGet(), 
											target->CoordinateGet());
		
		order::INFO status;
		
		if(	distance < current->gatAttackRange()){
			status = current->Attack(target);
			
			if(status == order::INFO::IS_DONE){
				Object *enemy = current->FindEnemy();
				if(enemy == nullptr)
					return order::INFO::IS_DONE;
				
				distance = geometry::Range(	current->CoordinateGet(), 
											enemy->CoordinateGet());
				
				if(distance <= current->gatVisibilityRange()){
					target = enemy;
					return order::INFO::EXERCISE;
				} else 
					return order::INFO::IS_DONE;
			} else 
				return order::INFO::EXERCISE;
		} else {
			status = current->Move(target->CoordinateGet());
			if(status == order::INFO::IMPOSSIBLY)
				return order::INFO::IMPOSSIBLY;
			else
				return order::INFO::EXERCISE;
		}
	}
	
	Hold::Hold(const Attributes&& entry)
				: position(entry.firstPosition){
	}
	
	order::INFO Hold::Do(Unit *current){
		Object* enemy = current->FindEnemy();
		
		if(enemy == nullptr)
			return order::INFO::EXERCISE;
		
		double distance = geometry::Range(	current->CoordinateGet(), 
											enemy->CoordinateGet());
		
		if(	distance < current->gatAttackRange()){
			current->Attack(enemy);
			return order::INFO::EXERCISE;
		} else
			return order::INFO::EXERCISE;	
	}
	
	Move::Move(const Attributes&& entry)
			: position(entry.firstPosition){
	}
	
	order::INFO Move::Do(Unit *current){
		if(position == current->CoordinateGet())
			return order::INFO::IS_DONE;
		else {
			order::INFO status = current->Move(position);
			
			if(status == order::INFO::IMPOSSIBLY)
				return order::INFO::IMPOSSIBLY;
			else
				return order::INFO::EXERCISE;
		}
	}
	
	Follow::Follow(const Attributes&& entry)
			: target(entry.target){
	}
	
	order::INFO Follow::Do(Unit *current){
		order::INFO status = current->Move(target->CoordinateGet());
		
		if(status == order::INFO::IMPOSSIBLY)
			return order::INFO::IMPOSSIBLY;
		else
			return order::INFO::EXERCISE;
	}
	
	void Patrol::swapPosition(){
		if(*currentPosition == firstPosition)
				*currentPosition = secondPosition;
			else
				*currentPosition = firstPosition;
	}
	
	Patrol::Patrol(const Attributes&& entry)
			:	firstPosition(entry.firstPosition), 
				secondPosition(entry.secondPosition){
		currentPosition = &firstPosition;
	}
	
	order::INFO Patrol::Do(Unit *current){
		order::INFO status = current->Move(*currentPosition);
		if(status == order::INFO::IS_DONE){	
			this->swapPosition();
			return order::INFO::EXERCISE;	
		} else if(status == order::INFO::IMPOSSIBLY)	
			return order::INFO::IMPOSSIBLY;	
		else 
			return order::INFO::EXERCISE;
	}
	
}