#ifndef ROYAL_MARKET_H
#define ROYAL_MARKET_H

#include "royal_market_Types.h"

void Royal_Customer_init(Royal_Customer* customer, double cash);

Royal_CustomerPos* Royal_Customer_get(const Royal_Customer* customer, Royal_Id id);

#define ROYAL_CUSTOMER_GET(cust, id) (Royal_IdBuf_get(&(cust->positions), id))

int Royal_Customer_deposit(Royal_Customer* customer, 
	                        Royal_Id id,
	                        double cost_basis,
	                        double amount);


Royal_MarketStatus Royal_Market_init(Royal_Market* market, Royal_MarketVotality vol);

Royal_Customer* Royal_Market_get_customer(const Royal_Market* m, Royal_Id id);

Royal_MarketStatus Royal_Market_make_customer(Royal_Market* m, const char* name, double start_cash);
// PRIVATE Royal_Id Royal_Market_make_item(Royal_Market* m, const char* name, double start_price);

// int Royal_Market_deposit_cash(Royal_Market* m, Royal_Id cust_id, double amount)
// \ ditto const char* for id
// int Royal_Market_deposit_item(Royal_Market* m, Royal_Id cust_id, Royal_Id item_id, double amount, double price)
// \ ditto const char* for id

/// type should split by market / limit and buy / sell
// int Royal_Market_make_order(Royal_Market* m, Royal_Id cust_id, Royal_Id item_id, Royal_OrderType type)

// int Royal_Market_exec_orders(Royal_Market* m, size_t* max);

/****
 Various query API's
 *****/

// possibly writes to a buffer?
// void Royal_Market_repr(const Royal_Market* m);




#endif // ROYAL_MARKET_H
