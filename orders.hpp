#ifndef ORDERS__
#define ORDERS__

#include "geometry.hpp"
#include "base_objects.hpp"
#include "order_info.hpp"
#include "order_subordinate.hpp"

#include <memory>

class Object;
class Destructible_object;

namespace order{
	
	struct Attributes;
	class Subordinate;
	
	class Order{
	protected:
		Order() = default;
	public:	
		virtual INFO Check(Subordinate *current) = 0;
		
	};
	
	class Default : public Order{
	private:
		geometry::Point position;
		
	public:
		Default(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};

	class Attack : public Order{
	private:
		std::shared_ptr<Destructible_object> target;
	public:
		Attack(const Attributes&& entry);
	
		INFO Check(Subordinate *current) override;
	};
	
	class Hold : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Hold(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};

	class Move : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Move(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};

	class Follow : public Order{
	private:
		std::shared_ptr<Destructible_object> target;
	public:
		Follow(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};
	
	class Patrol : public Order{
		using Point = geometry::Point;
	private:
		Point firstPosition;
		Point secondPosition;
		
		Point *currentPosition;
		
		void swapPosition();
		
	public:
		Patrol(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};
	
}
#endif