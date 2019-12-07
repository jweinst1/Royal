#ifndef ROYAL_MARKET_H
#define ROYAL_MARKET_H

#include "royal_ids.h"

typedef struct {
	Royal_Id item;
	double cost_basis;
	double amount;
} Royal_CustomerPosition;

typedef struct {
	Royal_CustomerPosition* items;
	double cash;
} Royal_Customer;


typedef struct {

} Royal_Market;

#endif // ROYAL_MARKET_H