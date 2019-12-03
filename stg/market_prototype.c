#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Preliminary prototype of market model
 */

#define MAX_ITEM_ID 64
#define NO_BUY_DEFAULT 0.1
#define VOTALITY_LEVEL 1

typedef struct {
	double positions[MAX_ITEM_ID];
	double cash;
} customer_t;

struct order_t {
	double price;
	double amount;
	customer_t* customer;
	struct order_t* next;
};
typedef struct order_t order_t;

typedef struct {
	order_t* buys_back;
	order_t* buys_front;
	order_t* sells_back;
	order_t* sells_front;
} order_queue_t;

typedef struct {
	size_t id;
	double cur_price;
	order_queue_t orders;
} item_t;

static void order_queue_insert(order_queue_t* q, order_t* o, int sell)
{
	order_t** back = sell ? &(q->sells_back) : &(q->buys_back);
	order_t** front = sell ? &(q->sells_front) : &(q->buys_front);
	if(*front == NULL) {
		*front = o;
	} else if(*back == NULL) {
		*back = o;
		(*front)->next = o;
	} else {
		o->next = NULL;
		(*back)->next = o;
		*back = o;
	}
}

static order_t* order_queue_pop(order_queue_t* q, int sell)
{
	order_t** back = sell ? &(q->sells_back) : &(q->buys_back);
	order_t** front = sell ? &(q->sells_front) : &(q->buys_front);
	if(*front == NULL) {
		return NULL;
	} else if(*back == NULL) {
		order_t* popped = *front;
		*front = NULL;
		return popped;
	} else {
		order_t* popped = *front;
		*front = *back;
		*back = (*back)->next;
		return popped;
	}
}

static double order_queue_sum(order_queue_t* q, int sell)
{
	double total = 0.0;
	order_t* ptr = sell ? q->sells_front : q->buys_front;
	while(ptr != NULL) {
		total += ptr->amount; // todo add price
		ptr = ptr->next;
	}
	return total;
}

static void order_del(order_t* q)
{
	if(q != NULL)
		order_del(q->next);
	free(q);
}

static void order_queue_clear(order_queue_t* q)
{
	order_del(q->buys_front);
	order_del(q->sells_front);
	memset(q, 0, sizeof(order_queue_t));
}

static double item_calc_price(item_t* item)
{
	double buy_total = order_queue_sum(&(item->orders), 0);
	double sell_total = order_queue_sum(&(item->orders), 1);
	if(buy_total == sell_total)
		// no price change
		return item->cur_price;
	sell_total = sell_total == 0.0 ? 1.0 : sell_total;
	buy_total = buy_total == 0.0 ? NO_BUY_DEFAULT : buy_total;
	return item->cur_price * (buy_total / sell_total);
}

static void item_change_price(item_t* item)
{
	double price_delta = item_calc_price(item) - item->cur_price;
	item->cur_price += price_delta / VOTALITY_LEVEL;
}

static void item_process_orders(item_t* item)
{
	printf("Processing Orders for item %zu\n", item->id);
	order_t* buy_ptr = item->orders.buys_front;
	order_t* sell_ptr = item->orders.sells_front;
	while(buy_ptr != NULL && sell_ptr != NULL) {
		printf("BUYING: %p customer_Cash: %f, amount %f\n", buy_ptr->customer, buy_ptr->customer->cash, buy_ptr->amount);
		printf("SELLING: %p customer_Cash %f, amount %f\n", sell_ptr->customer,  sell_ptr->customer->cash, sell_ptr->amount);
		double amount_buyable = (buy_ptr->customer->cash) / item->cur_price;
		// needed incase of price bump
		amount_buyable = (amount_buyable > buy_ptr->amount) ? buy_ptr->amount : amount_buyable;
		double transact_amount = amount_buyable > sell_ptr->amount ? sell_ptr->amount : amount_buyable;
		double transact_cash_paid = transact_amount * item->cur_price;
		buy_ptr->customer->cash -= transact_cash_paid;
		sell_ptr->customer->cash += transact_cash_paid;
		buy_ptr->customer->positions[item->id] += transact_amount;
		sell_ptr->customer->positions[item->id] -= transact_amount;
		buy_ptr->amount -= transact_amount;
		sell_ptr->amount -= transact_amount;
		if(buy_ptr->amount == 0.0) /// might change to threshhold
			buy_ptr = buy_ptr->next;
		if(sell_ptr-> amount == 0.0)
			sell_ptr = sell_ptr->next;
	}
}

item_t* item_new(size_t id, double start_price)
{
	item_t* it = calloc(1, sizeof(item_t));
	it->cur_price = start_price;
	it->id = id;
	return it;
}

order_t* order_new(double amount, double price, customer_t* customer)
{
	order_t* ord = calloc(1, sizeof(order_t));
	ord->amount = amount;
	ord->price = price;
	ord->customer = customer;
	return ord;
}

void item_place_order(item_t* item, customer_t* customer, double amount, /*to do add price*/int sell)
{
	double cash_payable = amount * item->cur_price;
	if(cash_payable > customer->cash) {
		fprintf(stderr, "Customer %p wanted to pay $%f, but only hs $%f\n", cash_payable, customer->cash);
		exit(3);
	}
	order_t* created = order_new(amount, item->cur_price, customer);
	order_queue_insert(&(item->orders), created, sell);
}

void customer_init(customer_t* customer, double cash)
{
	memset(customer->positions, 0, sizeof(customer->positions));
	customer->cash = cash;
}

void item_print(const item_t* item)
{
	const order_t* buy_orders;
	const order_t* sell_orders;
	puts("----------------------------------");
	printf("Item Id: %zu\n", item->id);
	printf("Item Price: %f\n", item->cur_price);
	puts("______________orders_______________");
	buy_orders = item->orders.buys_front;
	sell_orders = item->orders.sells_front;
	puts("Buying Orders:");
	while(buy_orders != NULL) {
		printf("Amount: %f, Price: %f, Customer %p\n", 
			                  buy_orders->amount,
			                  buy_orders->price,
			                  buy_orders->customer);
		buy_orders = buy_orders->next;
	}
	puts("Selling Orders:");
	while(sell_orders != NULL) {
		printf("Amount: %f, Price: %f, Customer %p\n", 
			                  sell_orders->amount,
			                  sell_orders->price,
			                  sell_orders->customer);
		sell_orders = sell_orders->next;
	}
	puts("-----------------------------------");
}

static customer_t XC1;
static customer_t XC2;
static customer_t XC3;

static void xc_init(void)
{
	customer_init(&XC1, 70.0);
	customer_init(&XC2, 200.0);
	customer_init(&XC3, 600.0);
	XC1.positions[0] = 70.0;
	XC1.positions[1] = 54.7;
	XC2.positions[0] = 9.0;
	XC2.positions[1] = 40.3;
	XC2.positions[2] = 130.0; 
}


int main(int argc, char const *argv[])
{
	item_t* ITEM0 = item_new(0, 10.0);
	item_t* ITEM1 = item_new(1, 6.0);
	item_t* ITEM2 = item_new(2, 8.0);
	puts("Market Simulation");
	xc_init();
	item_place_order(ITEM0, &XC1, 25.0, 1);
	item_place_order(ITEM0, &XC3, 20.0, 0);
	item_place_order(ITEM1, &XC2, 11.0, 0);
	item_place_order(ITEM1, &XC1, 20.0, 1);
	item_place_order(ITEM1, &XC3, 16.0, 0);
	item_print(ITEM0);
	item_print(ITEM1);
	item_change_price(ITEM0);
	item_change_price(ITEM1);
	item_process_orders(ITEM0);
	item_process_orders(ITEM1);
	item_print(ITEM0);
	item_print(ITEM1);
	return 0;
}

/**
Market Simulation
----------------------------------
Item Id: 0
Item Price: 10.000000
______________orders_______________
Buying Orders:
Amount: 20.000000, Price: 10.000000, Customer 00007FF6068ADFF0
Selling Orders:
Amount: 25.000000, Price: 10.000000, Customer 00007FF6068ADBD0
-----------------------------------
----------------------------------
Item Id: 1
Item Price: 6.000000
______________orders_______________
Buying Orders:
Amount: 11.000000, Price: 6.000000, Customer 00007FF6068ADDE0
Amount: 16.000000, Price: 6.000000, Customer 00007FF6068ADFF0
Selling Orders:
Amount: 20.000000, Price: 6.000000, Customer 00007FF6068ADBD0
-----------------------------------
Processing Orders for item 0
BUYING: 00007FF6068ADFF0 customer_Cash: 600.000000, amount 20.000000
SELLING: 00007FF6068ADBD0 customer_Cash 70.000000, amount 25.000000
Processing Orders for item 1
BUYING: 00007FF6068ADDE0 customer_Cash: 200.000000, amount 11.000000
SELLING: 00007FF6068ADBD0 customer_Cash 230.000000, amount 20.000000
BUYING: 00007FF6068ADFF0 customer_Cash: 440.000000, amount 16.000000
SELLING: 00007FF6068ADBD0 customer_Cash 319.100000, amount 9.000000
----------------------------------
Item Id: 0
Item Price: 8.000000
______________orders_______________
Buying Orders:
Amount: 0.000000, Price: 10.000000, Customer 00007FF6068ADFF0
Selling Orders:
Amount: 5.000000, Price: 10.000000, Customer 00007FF6068ADBD0
-----------------------------------
----------------------------------
Item Id: 1
Item Price: 8.100000
______________orders_______________
Buying Orders:
Amount: 0.000000, Price: 6.000000, Customer 00007FF6068ADDE0
Amount: 7.000000, Price: 6.000000, Customer 00007FF6068ADFF0
Selling Orders:
Amount: 0.000000, Price: 6.000000, Customer 00007FF6068ADBD0
-----------------------------------
**/