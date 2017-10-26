#ifndef NEW_ORDERS__
#define NEW_ORDERS__

#include "geometry.hpp"
#include "base_objects.hpp"
#include "order_subordinate.hpp"
#include "order_info.hpp"

namespace order{
	struct Attributes;
	class Subordinate;
}

namespace norder{
	
	class Order{
	protected:		
		Order() = default;
	public:	
		virtual order::INFO Do(order::Subordinate *current, float time) = 0;
		
	};
	
	class Default : public Order{
	private:
		enum class STATE{
			STAND,
			TRY_ATTACK,
			ATTACK,
			FOLLOW,
			GET_BACK
		} 					state;
		geometry::Point 	position;
		
		bool EnemyFind(order::Subordinate *current, float time);
		bool EnemyPresence(order::Subordinate *current, float time);
		bool AttackRangeOut(order::Subordinate *current, float time);
		bool AttackRangeIn(order::Subordinate *current, float time);
		bool FollowRangeOut(order::Subordinate *current, float time);
		bool OnPosition(order::Subordinate *current, float time);
		
	public:
	
		Default(const order::Attributes&& entry);
		
		order::INFO Do(order::Subordinate *current, float time) override;
		
	};
	
	class Move : public Order{
	private:
		enum class STATE{
			START,
			MOVE
		} 					state;
		geometry::Point 	position;
		
		
	public:
		Move(const order::Attributes&& entry);
		
		order::INFO Do(order::Subordinate *current, float time) override;
		
	};
}

#endif