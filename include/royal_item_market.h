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

#endif // ROYAL_ITEM_MARKET_H