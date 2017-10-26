#ifndef ORDER_SUBORDINATE__
#define ORDER_SUBORDINATE__

#include "base_objects.hpp"
#include "new_orders.hpp"

#include <list>
#include <memory>

struct ObjectAttributes;

namespace norder{
	class Order;
}

namespace order{
	
	struct Attributes{
		using Point = geometry::Point;
		Point firstPosition;
		Point secondPosition;
		std::shared_ptr<Destructible_object> target;
	};
	
	class Subordinate : virtual public Object{
		using Order_ptr = std::unique_ptr<norder::Order>;
	private:
		std::list<Order_ptr> orders;
		
		void AddDefault();
		std::list<Order_ptr> CreateEmpty();
		void ReplaceOrders(std::list<Order_ptr> &&new_);
	public:
		Subordinate() = delete;
		Subordinate(const ObjectAttributes *attr);
		Subordinate(const Subordinate&) = delete;
		Subordinate(Subordinate&&) = default;
		
		void AddOrder(Order_ptr &&next);
		void NewOrder(Order_ptr &&new_);
		void ResetOrders();
		
		void Update(float time);
		
		virtual ~Subordinate() = default;
	};
	
}

#endif