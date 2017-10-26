#include "order_subordinate.hpp"

#include <list>
#include <memory>

namespace order{
	
//=======================SUBORDINATE=================================
	void Subordinate::AddDefault(){
		typedef Subordinate::Order_ptr Order_ptr;
		Attributes setting;
		setting.firstPosition = this->CoordinateGet();
		Order_ptr order = std::make_unique<norder::Default>(std::move(setting));
		this->orders.push_front(std::move(order));
	}
	
	std::list<Subordinate::Order_ptr> Subordinate::CreateEmpty(){
		typedef Subordinate::Order_ptr Order_ptr;
		std::list<Order_ptr> new_;
		return new_;
	}
	
	Subordinate::Subordinate(const ObjectAttributes *attr)
			:	Object(attr){
		this->orders = CreateEmpty();
		this->AddDefault();
	}
	
	void Subordinate::AddOrder(Order_ptr &&next){		
		this->orders.push_front(std::move(next));
	}
	
	void Subordinate::NewOrder(Order_ptr &&new_){
		this->orders = CreateEmpty();
		this->AddOrder(std::move(new_));
	}
	
	void Subordinate::ResetOrders(){
	//	this->orders = CreateDefault();
	}
	
	void Subordinate::Update(float time){
		order::INFO status = this->orders.back()->Do(this, time);
		
		if(status != order::INFO::EXERCISE){
			this->orders.pop_back();			
		}
		
		if(this->orders.empty())
			this->AddDefault();
		
	/*	while(status != order::INFO::EXERCISE){
			this->orders.pop_front();
			status = this->orders.front()->Do(this, time);
		}*/
	}
//-------------------------------------------------------------------

}