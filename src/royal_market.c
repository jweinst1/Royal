#include "royal_market.h"

void Royal_Customer_init(Royal_Customer* customer, double cash)
{
	ROYAL_IDBUF_INIT(&(customer->positions), Royal_CustomerPos);
	customer->cash = cash;
}

Royal_CustomerPos* Royal_Customer_get(const Royal_Customer* customer, Royal_Id id)
{
	return Royal_IdBuf_get(&(customer->positions), id);
}

int Royal_Customer_deposit(Royal_Customer* customer, 
	                        Royal_Id id,
	                        double cost_basis,
	                        double amount)
{
	Royal_CustomerPos* pos = Royal_IdBuf_get(&(customer->positions), id);
	if(pos == NULL)
		return 0;
	pos->cost_basis += cost_basis;
	pos->amount += amount;
	return 1;
}