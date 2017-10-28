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
		if(currSeeCast == nullptr)
			return false;
		
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
		if((currAttackCast == nullptr) ||(currMoveCast == nullptr))
			return false;
		
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
		if((currAttackCast == nullptr) ||(currMoveCast == nullptr))
			return false;
		
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
		if(currAttackCast == nullptr)
			return false;
		
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
		Able_to_move* currMoveCast = current->CanMove();
		
		if((currAttackCast == nullptr) ||(currMoveCast == nullptr))
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
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
		
		std::cerr << "Move::Do\n";
		if(state == Move::STATE::START){
			std::cerr << "Move::STATE::START\n";
			Able_to_move* currMoveCast = current->CanMove();
			if(currMoveCast == nullptr)
				return INFO::IMPOSSIBLY;
			
			currMoveCast->MoveUpdate(this->position);
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

//===========================ORDER_HOLD==============================
	Hold::Hold(const order::Attributes&& entry)
				:	state(Hold::STATE::START){		
	}
	
	bool Hold::EnemyFind(order::Subordinate *current, float time){
		std::cerr << "Hold::EnemyFind\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->FindEnemy();
		
		if(enemy == nullptr)
			return false;
		
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance <= currAttackCast->getAttackRange()){
			currAttackCast->setTarget(enemy);
			state = Hold::STATE::TRY_ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Hold::EnemyPresence(order::Subordinate *current, float time){
		std::cerr << "Hold::EnemyPresence\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		
		if(currAttackCast == nullptr)
			return false;
		
		if(!currAttackCast->getTarget()->getLive()){
			state = Hold::STATE::STAND;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Hold::AttackRangeOut(order::Subordinate *current, float time){
		std::cerr << "Hold::AttackRangeOut\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance > currAttackCast->getAttackRange()){
			state = Hold::STATE::STAND;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	order::INFO Hold::Do(order::Subordinate *current, float time){
		using order::INFO;
		
			std::cerr << "Hold::Do\n";
			if(this->state == Hold::STATE::STAND){
				std::cerr << "Hold::STATE::STAND\n";
				if(this->EnemyFind(current, time))
					return INFO::EXERCISE;
				return INFO::EXERCISE;
				
			} else if(this->state == Hold::STATE::TRY_ATTACK) {
				std::cerr << "Hold::STATE::TRY_ATTACK\n";
				if(EnemyPresence(current, time))
					return INFO::EXERCISE;
				if(AttackRangeOut(current, time))
					return INFO::EXERCISE;
				state = Hold::STATE::ATTACK;
				current->CanAttack()->AttackCancel();
				this->Do(current, time);
				return INFO::EXERCISE;
				
			} else if(this->state == Hold::STATE::ATTACK) {
				std::cerr << "Hold::STATE::ATTACK\n";
				
				if(EnemyPresence(current, time))
					return INFO::EXERCISE;			
				
				order::INFO res = current->CanAttack()->Attack(time);
				std::cerr << "Hold::->Attack\n";
				
				if(res == order::INFO::IS_DONE){
					state = Hold::STATE::TRY_ATTACK;
					return INFO::EXERCISE;
				} else
					return INFO::EXERCISE;
				
			} else if(this->state == Hold::STATE::START){
				std::cerr << "Hold::STATE::START\n";
				
				this->position = current->CoordinateGet();
				state = Hold::STATE::STAND;
				this->Do(current, time);
				return INFO::EXERCISE;
			}
	}
//-------------------------------------------------------------------

//===========================ORDER_PATROL============================
	Patrol::Patrol(const order::Attributes&& entry)
			:	state(Patrol::STATE::START),
				firstPosition(),
				secondPosition(entry.firstPosition){	
	}

	void Patrol::SwithPath(){
		std::cerr << "Patrol::SwithPath\n";
		currentPath = (*currentPath == firstPosition ? &secondPosition : &firstPosition);
	}
	
	bool Patrol::EnemyFind(order::Subordinate *current, float time){
		std::cerr << "Patrol::EnemyFind\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->FindEnemy();
		
		if(enemy == nullptr)
			return false;
		
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		double currDistance = Range(enemy->getObject()->CoordinateGet(), onPathPosition);
		double maxDistance = 	currAttackCast->getVisibilityRange()
								+ currAttackCast->getAttackRange();
								
		if(		(distance <= currAttackCast->getVisibilityRange())
			&& 	(currDistance < maxDistance)){
			currAttackCast->setTarget(enemy);
			state = Patrol::STATE::TRY_ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Patrol::CheckPath(order::Subordinate *current, float time){
		std::cerr << "Patrol::CheckPath\n";
		
		if(current->CoordinateGet() == *currentPath){
			this->SwithPath();
			current->CanMove()->MoveUpdate(*currentPath);
			this->Do(current, time);
			return true;
		} else 
			return false;		
	}
	
	bool Patrol::EnemyPresence(order::Subordinate *current, float time){
		std::cerr << "Patrol::EnemyPresence\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		if(currAttackCast == nullptr)
			return false;
		
		if(!currAttackCast->getTarget()->getLive()){
			state = Patrol::STATE::GET_BACK;
			current->CanMove()->MoveUpdate(this->onPathPosition);
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Patrol::AttackRangeOut(order::Subordinate *current, float time){
		std::cerr << "Patrol::AttackRangeOut\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(distance > currAttackCast->getAttackRange()){
			this->state = Patrol::STATE::FOLLOW;
			current->CanMove()->MoveUpdate(enemy->getObject()->CoordinateGet());
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Patrol::AttackRangeIn(order::Subordinate *current, float time){
		std::cerr << "Patrol::AttackRangeIn\n";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), current->CoordinateGet());
		
		if(	distance < currAttackCast->getAttackRange()){
			state = Patrol::STATE::TRY_ATTACK;
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Patrol::ToFar(order::Subordinate *current, float time){
		std::cerr << "Patrol::ToFar ";
		
		Able_to_attack* currAttackCast = current->CanAttack();
		if(currAttackCast == nullptr)
			return false;
		
		std::shared_ptr<Destructible_object> enemy = currAttackCast->getTarget();
		double distance = Range(enemy->getObject()->CoordinateGet(), this->onPathPosition);
		double maxDistance = 	currAttackCast->getVisibilityRange()
								+ currAttackCast->getAttackRange();
		std::cerr << "distance = " << distance << " max distance = " << maxDistance << std::endl;
		if(distance > maxDistance){
			state = Patrol::STATE::GET_BACK;
			current->CanMove()->MoveUpdate(onPathPosition);
			this->Do(current, time);
			return true;
		} else
			return false;
	}
	
	bool Patrol::ReturnedToPosition(order::Subordinate *current, float time){
		std::cerr << "Patrol::ReturnedToPosition\n";
		if(this->onPathPosition == current->CoordinateGet()){
			state = Patrol::STATE::MOVE;
			current->CanMove()->MoveUpdate(*currentPath);
			this->Do(current, time);
			return true;
		} else {
			return false;
		}
	}
	
	order::INFO Patrol::Do(order::Subordinate *current, float time){
		using order::INFO;
		
		if(state == Patrol::STATE::MOVE){
			std::cerr << "Patrol::STATE::MOVE\n";
			this->onPathPosition = current->CoordinateGet();
			
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			if(CheckPath(current, time))
				return INFO::EXERCISE;
			
			current->CanMove()->Move(time);
			std::cerr 	<< "Patrol::->Move to " << currentPath->x << ',' << currentPath->y 
						<< " distance: " << geometry::Range(*currentPath, current->CoordinateGet()) 
						<< std::endl;
			return INFO::EXERCISE;
			
		} else if(state == Patrol::STATE::TRY_ATTACK) {
			
			std::cerr << "Patrol::STATE::TRY_ATTACK\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeOut(current, time))
				return INFO::EXERCISE;
			state = Patrol::STATE::ATTACK;
			current->CanAttack()->AttackCancel();
			this->Do(current, time);
			return INFO::EXERCISE;
			
		} else if(state == Patrol::STATE::ATTACK) {
			
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			
			std::cerr << "Patrol::STATE::ATTACK\n";
			order::INFO res = current->CanAttack()->Attack(time);
			std::cerr << "Patrol::->Attack\n";
			
			if(res == order::INFO::IS_DONE){
				state = Patrol::STATE::TRY_ATTACK;
				return INFO::EXERCISE;
			} else
				return INFO::EXERCISE;
			
		} else if(state == Patrol::STATE::FOLLOW) {
			
			std::cerr << "Patrol::STATE::FOLLOW\n";
			if(EnemyPresence(current, time))
				return INFO::EXERCISE;
			if(AttackRangeIn(current, time))
				return INFO::EXERCISE;
		//	if(AttackRangeOut(current, time))
		//		return INFO::EXERCISE;
			if(ToFar(current, time))
				return INFO::EXERCISE;
			
			current->CanMove()->Move(time);
			std::cerr 	<< "Patrol::->Move out " << onPathPosition.x << ',' << onPathPosition.y 
						<< " distance: " << geometry::Range(	onPathPosition, 
																current->CoordinateGet()) 
						<< std::endl;
			return INFO::EXERCISE;
			
		} else if(state == Patrol::STATE::GET_BACK) {
			
			std::cerr << "Patrol::STATE::GET_BACK\n";
			if(EnemyFind(current, time))
				return INFO::EXERCISE;
			if(ReturnedToPosition(current, time))
				return INFO::EXERCISE;
			
			std::cerr 	<< "Patrol::->Move to " << onPathPosition.x << ',' << onPathPosition.y 
						<< " distance: " << geometry::Range(	onPathPosition,
																current->CoordinateGet()) 
						<< std::endl;
			current->CanMove()->Move(time);
			
			return INFO::EXERCISE;
			
		} else if(state == Patrol::STATE::START) {
			
			std::cerr << "Patrol::STATE::START\n";
			if(!current->CanMove())
				return INFO::IMPOSSIBLY;
			
			firstPosition = current->CoordinateGet();
			currentPath = &secondPosition;
			current->CanMove()->MoveUpdate(*currentPath);
			state = Patrol::STATE::MOVE;
			Do(current, time);
			return INFO::EXERCISE;
		}
	}
//-------------------------------------------------------------------

//========================ORDER_ATTACK_HOLD==========================



//-------------------------------------------------------------------
}