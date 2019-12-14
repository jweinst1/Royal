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

Royal_MarketStatus Royal_Market_init(Royal_Market* market, Royal_MarketVotality vol)
{
	if(vol == 0)
		return ROYAL_MARKET_STATUS_ZERO_VOL;
	market->vol = vol;
	market->turn = 0;
	ROYAL_IDBUF_INIT(&(market->items), Royal_Item);
	ROYAL_IDBUF_INIT(&(market->customers), Royal_Customer);
	Royal_IdMap_init(&(market->item_names), NULL);
	Royal_IdMap_init(&(market->cust_names), NULL);
	return ROYAL_MARKET_STATUS_OK;
}

Royal_Customer* Royal_Market_get_customer(const Royal_Market* m, Royal_Id id)
{
	Royal_Customer* cust = Royal_IdBuf_get(&(m->customers), id);
	// Error Handling / setting
	return cust;
}

Royal_MarketStatus Royal_Market_make_customer(Royal_Market* m, const char* name, double start_cash)
{
	// Royal_Customer* cust;
	const Royal_Id* made = Royal_IdMap_add(&(m->cust_names), name);
	if(made == NULL)
		return ROYAL_MARKET_STATUS_CUST_AEXIST;
	// todo :: Royal_IdBuf_put(&(m->customers), *made, );
	return ROYAL_MARKET_STATUS_OK;
}