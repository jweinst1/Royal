#ifndef ROYAL_MARKET_H
#define ROYAL_MARKET_H

#include "royal_ids.h"

typedef struct {
	double cost_basis;
	double amount;
} Royal_CustomerPos;

typedef struct {
	Royal_IdBuf positions;
	double cash;
} Royal_Customer;

typedef struct {
	Royal_Customer* customer;
	double price;
	double amount;
} Royal_Order;

typedef struct {
	Royal_Order* orders;
	size_t len;
	size_t cap;
} Royal_OrderQueue;

typedef struct {

} Royal_Item;

typedef struct {
	Royal_IdBuf items;
	Royal_IdMap item_names;
} Royal_Market;

#endif // ROYAL_MARKET_H