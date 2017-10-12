#ifndef ORDERS__
#define ORDERS__

#include "geometry.hpp"
#include "objects.hpp"

namespace order{

	enum class INFO{
		IS_DONE,
		EXERCISE,
		IMPOSSIBLY
	};
	
	class Object;
	class Unit;
	
	struct Attributes{
		using geometry::Point Point;
		Point firstPosition;
		Point secondPosition;
		Object *target;
	};

	class Order{
	private:
		Order() = delete;
	//	Order(const Attributes& entry);
		
		virtual INFO Do(Unit *current);
		
		~Order() = 0;
	};

	class Attack : public Order{
	private:
		Object *target;
	public:
		Attack(const Attributes&& entry)
				: target(entry.target){
		}
	
		INFO Do(Unit *current) override{
			double distance = geometry::Range(	current->CoordinateGet(), 
												target->CoordinateGet());
			
			INFO status;
			
			if(	distance < current->gatAttackRange()){
				status = current->Attack(target);
				
				if(status == INFO::IS_DONE){
					Object *enemy = current->FindEnemy();
					if(enemy == nullptr)
						return INFO::IS_DONE;
					
					distance = geometry::Range(	current->CoordinateGet(), 
												enemy->CoordinateGet());
					
					if(distance <= current.gatVisibilityRange()){
						target = enemy;
						return INFO::EXERCISE;
					} else 
						return INFO::IS_DONE;
				}
			} else {
				status = current->Move(target->CoordinateGet());
				if(status == INFO::IMPOSSIBLY)
					return INFO::IMPOSSIBLY;
				else
					return INFO::EXERCISE;
			}
		}
	};
	
	class Hold : public Order{
		using geometry::Point Point;
	private:
		Point position;
		
	public:
		Hold(const Attributes&& entry)
				: position(entry.firstPosition){
		}
		
		INFO Do(Unit *current) override{
			Object* enemy = current->FindEnemy();
			
			if(enemy == nullptr)
				return INFO::EXERCISE;
			
			double distance = geometry::Range(	current->CoordinateGet(), 
												target->CoordinateGet())
			
			if(	distance < current->gatAttackRange()){
				current->Attack(enemy);
				return INFO::EXERCISE;
			} else
				return INFO::EXERCISE;	
		}
	};

	class Move : public Order{
		using geometry::Point Point;
	private:
		Point position;
		
	public:
		Move(const Attributes&& entry)
				: position(entry.firstPosition){
		}
		
		INFO Do(Unit *current) override{
			if(position == current->CoordinateGet())
				return INFO::IS_DONE;
			else {
				INFO status = current->Move(position);
				
				if(status == INFO::IMPOSSIBLY)
					return INFO::IMPOSSIBLY;
				else
					return INFO::EXERCISE;
				
			//	return status == INFO::IMPOSSIBLY ? 
			//			INFO::IMPOSSIBLY : INFO::EXERCISE;
			}
		}
	};

	class Follow : public Order{
	private:
		Object *target;
	public:
		Follow(const Attributes&& entry)
				: target(entry.target){
		}
		
		INFO Do(Unit *current) override{
			INFO status = current->Move(position);
			
			if(status == INFO::IMPOSSIBLY)
				return INFO::IMPOSSIBLY;
			else
				return INFO::EXERCISE;
		}
	};
	
	class Patrol : public Order{
		using geometry::Point Point;
	private:
		Point firstPosition;
		Point secondPosition;
		
		Point *currentPosition;
		
		void swapPosition(){
			if(*currentPosition == firstPosition)
					*currentPosition = secondPosition;
				else
					*currentPosition = firstPosition;
		}
		
	public:
		Patrol(const Attributes&& entry)
				:	firstPosition(entry.firstPosition), 
					secondPosition(entry.secondPosition){
			currentPosition = &firstPosition;
		}
		
		INFO Do(Unit *current) override{
			INFO status = current->Move(*currentPosition);
			if(status == INFO::IS_DONE){	
				this->swapPosition();
				return INFO::EXERCISE;	
			} else if(status == INFO::IMPOSSIBLY)	
				return INFO::IMPOSSIBLY;	
			else 
				return INFO::EXERCISE;
		}
	};
	
}
#endif