#include "new_orders.hpp"

#include "geometry.hpp"

#include <memory>

namespace norder{

//=======================ORDER_DEFAULT===============================
	Default::Default(const order::Attributes&& entry)
			:	position(entry.firstPosition), 
				state(Default::STATE::STAND){		
	}

	bool Default::EnemyFind(order::Subordinate *current, float time){
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
		auto currAttackCast = dynamic_cast<Able_to_attack*>(current);
		
		if(!currAttackCast->getTarget()->getLive()){
			state = Default::STATE::GET_BACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Default::AttackRangeOut(order::Subordinate *current, float time){
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
	
	bool Default::FollowRangeOut(order::Subordinate *current, float time){
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
		if(this->position == current->CoordinateGet()){
			state = Default::STATE::STAND;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	order::INFO Default::Do(order::Subordinate *current, float time){
		using order::INFO;
		
		if(state == Default::STATE::STAND){
			
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::ATTACK){
			
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeOut(current, time))
				return INFO::EXERCISE;
			dynamic_cast<Able_to_attack*>(current)->Attack(time);
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::FOLLOW){
			
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(!AttackRangeOut(current, time))
				return INFO::EXERCISE;
			if(FollowRangeOut(current, time))
				return INFO::EXERCISE;
			dynamic_cast<Able_to_move*>(current)->Move(time);
			return INFO::EXERCISE;
			
		} else if(state == Default::STATE::GET_BACK){
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			if(OnPosition(current, time))
				return INFO::EXERCISE;
			dynamic_cast<Able_to_move*>(current)->Move(time);
			
			return INFO::EXERCISE;
		}
	}
//-------------------------------------------------------------------

	
}