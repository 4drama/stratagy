#include "new_orders.hpp"

#include "geometry.hpp"

#include <memory>
#include <iostream>


namespace norder{

//=======================ORDER_DEFAULT===============================
	Default::Default(const order::Attributes&& entry)
			:	position(entry.firstPosition), 
				state(Default::STATE::STAND){		
	}

	bool Default::EnemyFind(order::Subordinate *current, float time){
		std::cerr << "Default::EnemyFind\n"; 
		Able_to_see* currSeeCast = current->CanSee();
		std::shared_ptr<Destructible_object> enemy = currSeeCast->FindEnemy();
		
		if(enemy == nullptr)
			return false;
		
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance <= currSeeCast->getVisibilityRange()){
			current->CanAttack()->setTarget(enemy);
			state = Default::STATE::TRY_ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::EnemyPresence(order::Subordinate *current, float time){
		std::cerr << "Default::EnemyPresence\n";
		Able_to_attack* currAttackCast = current->CanAttack();
		Able_to_move* currMoveCast = current->CanMove();
		
		if(!currAttackCast->getTarget()->getLive()){
			state = Default::STATE::GET_BACK;
			currMoveCast->MoveUpdate(this->position);
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::AttackRangeOut(order::Subordinate *current, float time){
		std::cerr << "Default::AttackRangeOut\n";
		Able_to_attack* currAttackCast = current->CanAttack();
		Able_to_move* currMoveCast = current->CanMove();
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance > currAttackCast->getAttackRange()){
			state = Default::STATE::FOLLOW;
			currMoveCast->MoveUpdate(enemy->getObject()->CoordinateGet());
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::AttackRangeIn(order::Subordinate *current, float time){
		std::cerr << "Default::AttackRangeIn\n";
		Able_to_attack* currAttackCast = current->CanAttack();
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance < currAttackCast->getAttackRange()){
			state = Default::STATE::TRY_ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::FollowRangeOut(order::Subordinate *current, float time){
		std::cerr << "Default::FollowRangeOut\n";
		Able_to_attack* currAttackCast = current->CanAttack();
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		Able_to_move* currMoveCast = current->CanMove();
		
		if(distance > currAttackCast->getVisibilityRange()){						
			state = Default::STATE::GET_BACK;
			currMoveCast->MoveUpdate(this->position);
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::OnPosition(order::Subordinate *current, float time){
		std::cerr << "Default::OnPosition\n";
		if(this->position == current->CoordinateGet()){
			state = Default::STATE::STAND;
			this->Do(current, time);
			return true;
		} else {
			return false;
		}
	}
	
	order::INFO Default::Do(order::Subordinate *current, float time){
		using order::INFO;
		
		std::cerr << "Default::Do\n";
		if(state == Default::STATE::STAND){
			std::cerr << "Default::STATE::STAND\n";
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			return INFO::EXERCISE;
			
		}else if(state == Default::STATE::TRY_ATTACK){
			std::cerr << "Default::STATE::TRY_ATTACK\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeOut(current, time))
				return INFO::EXERCISE;
			state = Default::STATE::ATTACK;
			current->CanAttack()->AttackCancel();
			this->Do(current, time);
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::ATTACK){

			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			
			std::cerr << "Default::STATE::ATTACK\n";
			order::INFO res = current->CanAttack()->Attack(time);
			std::cerr << "Default::->Attack\n";
			
			if(res == order::INFO::IS_DONE){
				state = Default::STATE::TRY_ATTACK;
				return INFO::EXERCISE;
			} else
				return INFO::EXERCISE;
			
		} else if(state == Default::STATE::FOLLOW){
			std::cerr << "Default::STATE::FOLLOW\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeIn(current, time))
				return INFO::EXERCISE;
			if(FollowRangeOut(current, time))
				return INFO::EXERCISE;
			current->CanMove()->Move(time);
			std::cerr 	<< "Default::->Move to " << position.x << ',' << position.y 
						<< " distance: " << geometry::Range(position, current->CoordinateGet()) 
						<< std::endl;
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::GET_BACK){
			std::cerr << "Default::STATE::GET_BACK\n";
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			if(OnPosition(current, time))
				return INFO::EXERCISE;
			std::cerr 	<< "Default::->Move to " << position.x << ',' << position.y 
						<< " distance: " << geometry::Range(position, current->CoordinateGet()) 
						<< std::endl;
			current->CanMove()->Move(time);
			
			return INFO::EXERCISE;
		}
	}
	
//-------------------------------------------------------------------

//================================ORDER_MOVE=========================
	Move::Move(const order::Attributes&& entry)
				:	position(entry.firstPosition), 
					state(Move::STATE::START){		
	}

	order::INFO Move::Do(order::Subordinate *current, float time){
		using order::INFO;
		
		if(state == Move::STATE::START){
			std::cerr << "Move::STATE::START\n";
			current->CanMove()->MoveUpdate(this->position);
			state = Move::STATE::MOVE;
			return this->Do(current, time);
			
		} else if(state == Move::STATE::MOVE){
			std::cerr << "Move::STATE::MOVE\n";
			std::cerr 	<< "Move::->Move to " << position.x << ',' << position.y 
						<< " distance: " << geometry::Range(position, current->CoordinateGet()) 
						<< std::endl;
			current->CanMove()->Move(time);
			if(current->CoordinateGet() == this->position)
				return INFO::IS_DONE;
			else 
				return INFO::EXERCISE;
		}
	}
//-------------------------------------------------------------------
}