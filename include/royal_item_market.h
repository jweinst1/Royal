#ifndef ROYAL_ITEM_MARKET_H
#define ROYAL_ITEM_MARKET_H

#include "royal_mem.h"

typedef struct {
	unsigned int size;
	unsigned long turns;
	unsigned int n_customers;
	unsigned int n_items;
	unsigned int max_orders;
	unsigned int max_strategies;
	unsigned char state;
	int order_state;
	int strategy_state;
	double votality;
} Royal_ItemMarket_Header;

typedef struct {
	double cash;
	int state;
	char name[32];
} Royal_ItemMarket_Customer;

typedef struct {
	unsigned char cmp_op;
	unsigned int cmp_item;
	double cmp_price;
	unsigned char action_type;
	unsigned int action_item;
	double action_amount;
	double action_price;
} Royal_ItemMarket_Strategy;

typedef struct {
	double amount;
	double cost_basis;
} Royal_ItemMarket_Position;

typedef struct {
	double current_price;
	int state;
	char name[32];
} Royal_ItemMarket_Item;

typedef struct {
	unsigned char type;
	unsigned int customer;
	double amount;
	double price;
} Royal_ItemMarket_Order;

#define ROYAL_ITEM_MARKET_ITEM_SIZE(orders) ((sizeof(Royal_ItemMarket_Order) * orders * 2) + sizeof(Royal_ItemMarket_Item))
#define ROYAL_ITEM_MARKET_ITEMS_SIZE(items, orders) (ROYAL_ITEM_MARKET_ITEM_SIZE(orders) * items)

#define ROYAL_ITEM_MARKET_CUST_SIZE(strats, items) ((sizeof(Royal_ItemMarket_Strategy) * strats)  \
                                                   + (sizeof(Royal_ItemMarket_Position) * items) \
                                                    + sizeof(Royal_ItemMarket_Customer))
#define ROYAL_ITEM_MARKET_CUSTS_SIZE(strats, items, custs) (ROYAL_ITEM_MARKET_CUST_SIZE(strats, items) * custs)

#define ROYAL_ITEM_MARKET_SIZE(items, custs, orders, strats) (sizeof(Royal_ItemMarket_Header) \
                                                              + ROYAL_ITEM_MARKET_CUSTS_SIZE(strats, items, custs) \
                                                              + ROYAL_ITEM_MARKET_ITEMS_SIZE(items, orders))

#endif // ROYAL_ITEM_MARKET_H