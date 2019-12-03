#ifndef ROYAL_CUSTOMER_H
#define ROYAL_CUSTOMER_H

typedef enum {
	ROYAL_CUSTOMER_TYPE_MARKET
} Ryl_CustomerType;

#define ROYAL_CUSTOMER_HEAD \
                    Ryl_CustomerType type; \
                    double cash

typedef struct {
	ROYAL_CUSTOMER_HEAD;
} Ryl_Customer;

#define ROYAL_CUSTOMER_CAST(obj) ((Ryl_Customer*)(obj))

#endif // ROYAL_CUSTOMER_H
