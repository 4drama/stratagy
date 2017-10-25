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
		auto currSeeCast = dynamic_cast<Able_to_see*>(current);
		std::shared_ptr<Destructible_object> enemy = currSeeCast->FindEnemy();		
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance <= currSeeCast->getVisibilityRange()){
			dynamic_cast<Able_to_attack*>(current)->setTarget(enemy);
			state = Default::STATE::ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::EnemyPresence(order::Subordinate *current, float time){
		std::cerr << "Default::EnemyPresence\n";
		auto currAttackCast = dynamic_cast<Able_to_attack*>(current);
		
		if(!currAttackCast->getTarget()->getLive()){
			state = Default::STATE::GET_BACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::AttackRangeOut(order::Subordinate *current, float time){
		std::cerr << "Default::AttackRangeOut\n";
		auto currAttackCast = dynamic_cast<Able_to_attack*>(current);
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance > currAttackCast->getAttackRange()){
			state = Default::STATE::FOLLOW;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::AttackRangeIn(order::Subordinate *current, float time){
		std::cerr << "Default::AttackRangeIn\n";
		auto currAttackCast = dynamic_cast<Able_to_attack*>(current);
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance < currAttackCast->getAttackRange()){
			state = Default::STATE::ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::FollowRangeOut(order::Subordinate *current, float time){
		std::cerr << "Default::FollowRangeOut\n";
		auto currAttackCast = dynamic_cast<Able_to_attack*>(current);
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance > currAttackCast->getVisibilityRange()){						
			state = Default::STATE::GET_BACK;
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
		} else
			return false;
	}
	
	order::INFO Default::Do(order::Subordinate *current, float time){
		using order::INFO;
		
		std::cerr << "Default::Do\n";
		if(state == Default::STATE::STAND){
			std::cerr << "Default::STATE::STAND\n";
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::ATTACK){
			std::cerr << "Default::STATE::ATTACK\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeOut(current, time))
				return INFO::EXERCISE;
			dynamic_cast<Able_to_attack*>(current)->Attack(time);
			std::cerr << "Default::->Attack\n";
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::FOLLOW){
			std::cerr << "Default::STATE::FOLLOW\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeIn(current, time))
				return INFO::EXERCISE;
			if(FollowRangeOut(current, time))
				return INFO::EXERCISE;
			dynamic_cast<Able_to_move*>(current)->Move(time);
			std::cerr << "Default::->Move\n";
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::GET_BACK){
			std::cerr << "Default::STATE::GET_BACK\n";
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			if(OnPosition(current, time))
				return INFO::EXERCISE;
			std::cerr << "Default::->Move\n";
			dynamic_cast<Able_to_move*>(current)->Move(time);
			
			return INFO::EXERCISE;
		}
	}
//-------------------------------------------------------------------

	
}