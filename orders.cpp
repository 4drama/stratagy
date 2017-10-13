#include "orders.hpp"

namespace order{
	
	Attack::Attack(const Attributes&& entry)
				: target(entry.target){
	}
	
	order::INFO Attack::Check(Subordinate *current){
		if(Able_to_attack *attacker = dynamic_cast<Able_to_attack*>(current)){
			double distance = geometry::Range(	attacker->CoordinateGet(), 
												target->CoordinateGet());
			
			order::INFO status;
			
			if(	distance < attacker->gatAttackRange()){
				status = attacker->Attack(target);
				
				if(status == order::INFO::IS_DONE){
					Object *enemy = attacker->FindEnemy();
					if(enemy == nullptr)
						return order::INFO::IS_DONE;
					
					distance = geometry::Range(	attacker->CoordinateGet(), 
												enemy->CoordinateGet());
					
					if(distance <= attacker->gatVisibilityRange()){
						target = enemy;
						return order::INFO::EXERCISE;
					} else 
						return order::INFO::IS_DONE;
				} else 
					return order::INFO::EXERCISE;
			} else {
				if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
					status = pedestrian->Move(pedestrian->CoordinateGet());
					if(status == order::INFO::IMPOSSIBLY)
						return order::INFO::IMPOSSIBLY;
					else
						return order::INFO::EXERCISE;
				}
				else
					return order::INFO::IMPOSSIBLY;
			}
		}
		return order::INFO::IMPOSSIBLY;
	}
	
	Hold::Hold(const Attributes&& entry)
				: position(entry.firstPosition){
	}
	
	order::INFO Hold::Check(Subordinate *current){
		if(Able_to_attack *attacker = dynamic_cast<Able_to_attack*>(current)){
			Object* enemy = attacker->FindEnemy();
			if(enemy == nullptr)
				return order::INFO::EXERCISE;
		
			double distance = geometry::Range(	attacker->CoordinateGet(), 
												enemy->CoordinateGet());
			
			if(	distance < attacker->gatAttackRange()){
				attacker->Attack(enemy);
				return order::INFO::EXERCISE;
			} else
				return order::INFO::EXERCISE;	
		} else
			return order::INFO::EXERCISE;
	}
	
	Move::Move(const Attributes&& entry)
			: position(entry.firstPosition){
	}
	
	order::INFO Move::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			if(position == pedestrian->CoordinateGet())
				return order::INFO::IS_DONE;
			else {
				order::INFO status = pedestrian->Move(position);
				
				if(status == order::INFO::IMPOSSIBLY)
					return order::INFO::IMPOSSIBLY;
				else
					return order::INFO::EXERCISE;
			}
		} else 
			return order::INFO::IMPOSSIBLY;
	}
	
	Follow::Follow(const Attributes&& entry)
			: target(entry.target){
	}
	
	order::INFO Follow::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			order::INFO status = pedestrian->Move(target->CoordinateGet());
			
			if(status == order::INFO::IMPOSSIBLY)
				return order::INFO::IMPOSSIBLY;
			else
				return order::INFO::EXERCISE;
		} else
			return order::INFO::IMPOSSIBLY;
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
	
	order::INFO Patrol::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			order::INFO status = pedestrian->Move(*currentPosition);
			if(status == order::INFO::IS_DONE){	
				this->swapPosition();
				return order::INFO::EXERCISE;	
			} else if(status == order::INFO::IMPOSSIBLY)	
				return order::INFO::IMPOSSIBLY;	
			else 
				return order::INFO::EXERCISE;
		} else 
			return order::INFO::IMPOSSIBLY;
	}
	
}