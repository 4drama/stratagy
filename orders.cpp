#include "orders.hpp"

#include <stdexcept>
#include <cassert>
#include <string>

#include <list>
#include <memory>

namespace order{


//===========================SUPPORTING==============================
	namespace{
		enum class STATUS{
			NOT_ATTACKER,
			OUT_OF_DISTANCE,
			TARGET_ATTACK,
			CANT_MOVE,
			CAN_MOVE
		};
		
		order::STATUS TryMove(Subordinate *current, geometry::Point position){
			Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current);
			if(pedestrian != nullptr){
				order::INFO status = pedestrian->MoveUpdate(position);
				if(status == order::INFO::IMPOSSIBLY)
					return order::STATUS::OUT_OF_DISTANCE;
				else
					return order::STATUS::CAN_MOVE;
			} else 
				return order::STATUS::CANT_MOVE;
		}
		
		
		order::STATUS TryAttack(	Subordinate *current,
						std::shared_ptr<Destructible_object> target){
			Able_to_attack *attacker = dynamic_cast<Able_to_attack*>(current);
			if(attacker != nullptr){
				double distance = geometry::Range(	attacker->CoordinateGet(), 
							target->getObject()->CoordinateGet());
				
				if(distance < attacker->getAttackRange()){
					order::INFO statusInfo = attacker->AttackUpdate(target);
					if(statusInfo == order::INFO::EXERCISE)
						return order::STATUS::TARGET_ATTACK;
					else if (statusInfo == order::INFO::IMPOSSIBLY)	
						return order::STATUS::OUT_OF_DISTANCE;
					else if(statusInfo == order::INFO::IS_DONE)
						return order::STATUS::OUT_OF_DISTANCE;
					else{
						std::string error_text = "ERROR::orders.cpp ";
						error_text += 		"TryAttack - statusInfo ";
						error_text +=		"unexpected value";
						throw std::logic_error(error_text);
					}
				} else 
					return order::STATUS::OUT_OF_DISTANCE;
			} else 
				return order::STATUS::NOT_ATTACKER; 
		}
	}
//-------------------------------------------------------------------

//===========================SUBORDINATE_CLASS=======================	
	std::list<Subordinate::Order_ptr> Subordinate::CreateDefault(){
		typedef Subordinate::Order_ptr Order_ptr;
		std::list<Order_ptr> new_;
		Attributes setting;
		setting.firstPosition = this->CoordinateGet();
		Order_ptr order = std::make_unique<Default>(std::move(setting));
		new_.push_front(std::move(order));
		return new_;
	}
	
	Subordinate::Subordinate(const ObjectAttributes *attr)
			:	Object(attr){
		this->orders = CreateDefault();
	}
	
	void Subordinate::AddOrder(Order_ptr &&next){
		this->orders.push_front(std::move(next));
	}
	
	void Subordinate::NewOrders(Order_ptr &&new_){
		this->orders = CreateDefault();
		this->AddOrder(std::move(new_));
	}
	
	void Subordinate::ResetOrders(){
		this->orders = CreateDefault();
	}
	
	void Subordinate::Update(){
		order::INFO status = this->orders.front()->Check(this);
		
		while(status != order::INFO::EXERCISE){
			this->orders.pop_front();
			status = this->orders.front()->Check(this);
		}
	}
//-------------------------------------------------------------------

//===========================DEFAULT_ORDER_CLASS=====================
	Default::Default(const Attributes&& entry)
				: position(entry.firstPosition){
		
	}
	
	namespace{
		order::STATUS DefaultPosCheck(		Able_to_move	*current,
							geometry::Point originalPosition,
							geometry::Point targetPosition){			
			double currDistance = geometry::Range(	originalPosition, 
									targetPosition);
			double maxDistance = current->getVisibilityRange();
			if(maxDistance > currDistance)
				return order::STATUS::CAN_MOVE;
			else 
				return order::STATUS::OUT_OF_DISTANCE;
		}
	}

	INFO Default::Check(Subordinate *current){		
		if(Able_to_see *caretaker = dynamic_cast<Able_to_see*>(current)){
			std::shared_ptr<Destructible_object> enemy = caretaker->FindEnemy();
			if(enemy != nullptr){
				
				order::STATUS status = TryAttack(current, enemy);
				
				if(		(status == order::STATUS::NOT_ATTACKER) 	||	
						(status == order::STATUS::TARGET_ATTACK)	)
					return order::INFO::EXERCISE;
					
				else if(status == order::STATUS::OUT_OF_DISTANCE){
					status = TryMove(current, enemy->getObject()->CoordinateGet());
					if(		(status == order::STATUS::CANT_MOVE)	||
							(status == order::STATUS::OUT_OF_DISTANCE))
						return order::INFO::EXERCISE;
						
					else if(status == order::STATUS::CAN_MOVE){
						Able_to_move *pedestrian;
						pedestrian = dynamic_cast<Able_to_move*>(current);

						//in this branch current object must can move.
						assert(pedestrian != nullptr);
						
						status = DefaultPosCheck(pedestrian, this->position,
								enemy->getObject()->CoordinateGet());
						if(status == order::STATUS::CAN_MOVE)
							return order::INFO::EXERCISE;
						else if(status == order::STATUS::OUT_OF_DISTANCE){
							status = TryMove(current, this->position);
							if(status == order::STATUS::CAN_MOVE)
								return order::INFO::EXERCISE;
							else if (status == order::STATUS::OUT_OF_DISTANCE){
								this->position = current->CoordinateGet();
								return order::INFO::EXERCISE;
							} else{
								std::string error_text;
								error_text = 	"ERROR::orders.cpp ";
								error_text += "Default::Check - TryMove: ";
								error_text += "incorrect branch.";
								throw std::logic_error(error_text);
							}
						} else {
							std::string error_text;
							error_text =  "ERROR::orders.cpp ";
							error_text += "Default::Check - DefaultPosCheck: ";
							error_text += "incorrect branch.";
							throw std::logic_error(error_text);
						}
					} else {
						std::string error_text;
						error_text = 	"ERROR::orders.cpp ";
						error_text += 	"Default::Check - TryMove: ";
						error_text += 	"incorrect branch.";
						throw std::logic_error(error_text);
					}
				} else {
					std::string error_text;
					error_text = 	"ERROR::orders.cpp ";
					error_text += 	"Default::Check - TryAttack: ";
					error_text +=	"incorrect branch.";
					throw std::logic_error(error_text);
				}
			} else
				return order::INFO::EXERCISE;
		} else
			return order::INFO::EXERCISE;
	}
//-------------------------------------------------------------------

//===========================ATTACK_ORDER_CLASS======================	
	Attack::Attack(const Attributes&& entry)
				: target(entry.target){
	}
	
	order::INFO Attack::Check(Subordinate *current){
		if(Able_to_attack *attacker = dynamic_cast<Able_to_attack*>(current)){
			double distance = geometry::Range(	attacker->CoordinateGet(), 
								target->getObject()->CoordinateGet());
			
			order::INFO status;
			
			if(	distance < attacker->getAttackRange()){
				status = attacker->AttackUpdate(target);
				
				if(status == order::INFO::IS_DONE){
					std::shared_ptr<Destructible_object> enemy; 
					enemy = attacker->FindEnemy();
					if(enemy == nullptr)
						return order::INFO::IS_DONE;
					
					distance = geometry::Range(	attacker->CoordinateGet(), 
								enemy->getObject()->CoordinateGet());
					
					if(distance <= attacker->getVisibilityRange()){
						target = enemy;
						return order::INFO::EXERCISE;
					} else 
						return order::INFO::IS_DONE;
				} else 
					return order::INFO::IS_DONE;
			} else {
				if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
					status = pedestrian->MoveUpdate(pedestrian->CoordinateGet());
					if(status == order::INFO::IMPOSSIBLY)
						return order::INFO::IMPOSSIBLY;
					else
						return order::INFO::EXERCISE;
				}
				else
					return order::INFO::IMPOSSIBLY;
			}
		} else
			return order::INFO::IMPOSSIBLY;
	}
//-------------------------------------------------------------------

//===========================HOLD_ORDER_CLASS========================	
	Hold::Hold(const Attributes&& entry)
			: 	position(entry.firstPosition){
	}
	
	order::INFO Hold::Check(Subordinate *current){
		if(Able_to_attack *attacker = dynamic_cast<Able_to_attack*>(current)){
			std::shared_ptr<Destructible_object> enemy = attacker->FindEnemy();
			if(enemy == nullptr)
				return order::INFO::EXERCISE;
		
			double distance = geometry::Range(	attacker->CoordinateGet(), 
								enemy->getObject()->CoordinateGet());
			
			if(	distance < attacker->getAttackRange()){
				attacker->AttackUpdate(enemy);
				return order::INFO::EXERCISE;
			} else
				return order::INFO::EXERCISE;	
		} else
			return order::INFO::EXERCISE;
	}
//-------------------------------------------------------------------

//===========================MOVE_ORDER_CLASS========================	
	Move::Move(const Attributes&& entry)
			:	 position(entry.firstPosition){
	}
	
	order::INFO Move::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			if(position == pedestrian->CoordinateGet())
				return order::INFO::IS_DONE;
			else {
				order::INFO status = pedestrian->MoveUpdate(position);
				
				if(status == order::INFO::IMPOSSIBLY)
					return order::INFO::IMPOSSIBLY;
				else
					return order::INFO::EXERCISE;
			}
		} else 
			return order::INFO::IMPOSSIBLY;
	}
//-------------------------------------------------------------------

//===========================FOLLOW_ORDER_CLASS======================	
	Follow::Follow(const Attributes&& entry)
			: 	target(entry.target){
	}
	
	order::INFO Follow::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			order::INFO status;
			status = pedestrian->MoveUpdate(target->getObject()->CoordinateGet());
			
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
//-------------------------------------------------------------------

//===========================PATROL_ORDER_CLASS======================	
	Patrol::Patrol(const Attributes&& entry)
			:	firstPosition(entry.firstPosition), 
				secondPosition(entry.secondPosition){
		currentPosition = &firstPosition;
	}
	
	order::INFO Patrol::Check(Subordinate *current){
		if(Able_to_move *pedestrian = dynamic_cast<Able_to_move*>(current)){
			order::INFO status = pedestrian->MoveUpdate(*currentPosition);
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
//-------------------------------------------------------------------
	
}