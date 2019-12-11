#ifndef ROYAL_MARKET_TYPES_H
#define ROYAL_MARKET_TYPES_H

#include "royal_ids.h"

typedef struct {
	double cost_basis;
	double amount;
} Royal_CustomerPos;

typedef struct {
	Royal_IdBuf positions;
	double cash;
} Royal_Customer;

typedef enum {
	ROYAL_ORDERTYPE_MARKET,
	ROYAL_ORDERTYPE_LIMIT
} Royal_OrderType;

typedef struct {
	Royal_Customer* customer;
	Royal_OrderType type;
	double price;
	double amount;
} Royal_Order;

typedef struct {
	Royal_Order* orders;
	size_t len;
	size_t cap;
} Royal_OrderQueue;

typedef struct {
	Royal_OrderQueue* buy_orders;
	Royal_OrderQueue* sell_orders;
	double current_price;
} Royal_Item;

typedef unsigned long Royal_MarketTurn;

typedef struct {
	Royal_IdBuf items;
	Royal_IdMap item_names;
	Royal_IdBuf customers;
	Royal_IdMap cust_names;
	Royal_MarketTurn turn;
} Royal_Market;

#endif // ROYAL_MARKEt_TYPES_H