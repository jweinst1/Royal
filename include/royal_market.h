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



#endif // ROYAL_MARKET_H
