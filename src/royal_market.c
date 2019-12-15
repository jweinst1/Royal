#include "royal_market.h"

#ifndef ROYAL_ORDERQUEUE_DEFAULT_CAP
#define ROYAL_ORDERQUEUE_DEFAULT_CAP 30
#endif

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

void Royal_OrderQueue_init(Royal_OrderQueue* q, size_t* capac)
{
	if(q->orders != NULL)
		return;
	size_t capacity = capac == NULL ? ROYAL_ORDERQUEUE_DEFAULT_CAP : *capac;
	_Royal_alloc(q->orders, capacity);
	q->len = 0;
	q->cap = capacity;
}

void Royal_OrderQueue_push(Royal_OrderQueue* q, Royal_Customer* cust,
	                                            Royal_OrderType type,
	                                            double amount,
	                                            double price)
{
	Royal_Order* ord;
	if(q->len == q->cap) {
#ifdef ROYAL_ORDERQUEUE_EXPON_GROWTH
		q->cap *= q->cap;
#else // ROYAL_ORDERQUEUE_EXPON_GROWTH
		q->cap *= 3;
#endif // !ROYAL_ORDERQUEUE_EXPON_GROWTH
		_Royal_realloc(q->orders, q->orders, q->cap);
	}
	ord = q->orders + q->len;
	ord->customer = cust;
	ord->type = type;
	ord->amount = amount;
	ord->price = price;
	q->len++;
}

void Royal_Item_init(Royal_Item* item, double start_price)
{
	Royal_OrderQueue_init(&(item->buy_orders), NULL);
	Royal_OrderQueue_init(&(item->sell_orders), NULL);
	item->current_price = start_price;
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
	Royal_Customer* cust;
	const Royal_Id* made = Royal_IdMap_add(&(m->cust_names), name);
	if(made == NULL)
		return ROYAL_MARKET_STATUS_AEXIST;
	cust = Royal_IdBuf_ret(&(m->customers), *made);
	cust->cash = start_cash;
	return ROYAL_MARKET_STATUS_OK;
}

Royal_MarketStatus Royal_Market_make_item(Royal_Market* m, const char* name, double start_price)
{
	Royal_Item* item;
	const Royal_Id* made = Royal_IdMap_add(&(m->item_names), name);
	if(made == NULL)
		return ROYAL_MARKET_STATUS_AEXIST;
	item = Royal_IdBuf_ret(&(m->items), *made);
	Royal_Item_init(item, start_price);
	return ROYAL_MARKET_STATUS_OK;
}

Royal_MarketStatus Royal_Market_make_order(Royal_Market* m, 
	                                       Royal_Id cust_id, 
	                                       Royal_Id item_id, 
	                                       Royal_OrderType type,
	                                       double amount,
	                                       double price)
{
	
	return ROYAL_MARKET_STATUS_OK;
}