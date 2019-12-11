#include "royal_market.h"

void Royal_Customer_init(Royal_Customer* customer, double cash)
{
	ROYAL_IDBUF_INIT(&(customer->positions), Royal_Customer);
	customer->cash = cash;
}