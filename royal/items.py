import struct

# - Size of the market (I)
# - Number of turns (L)
# - number of customers (I)
# - number of items (I)
# - max orders (I)
# - max strategies (I)
# - state of market (order or exec) (B)
# - state of item order execution (i, off is -1)
#-  state of strategy execution for customers (i, off is -1)
MARKET_HEADER = struct.Struct("ILIIIIBii")

# - current price (d)
# - current order state (i, off is -1), used for exec or
#   placing orders, when an order queue is full.
# - String name of item (32s, last must be \0)
ITEMS_SCHEMA = struct.Struct("di32s")
# - type of order (B)
# - offset of customer (I)
# - offset of item (I)
# - amount ordering (d)
# - price ordering (d)
# ~ Note: buy chunk of orders comes before sell chunk
ORDERS_SCHEMA = struct.Struct("BIIdd")
# - current strategy state (i, off is -1)
# - The cash possessed by the customer (d)
# - String name of customer (32s, last must be \0)
CUSTOMERS_SCHEMA = struct.Struct("id32s")
# - comparison type (B, such as gt, ge, lt, le etc.)
# - stock offset (I)
# - price argument (d)
# - action to take if condition is true (B -> order type)
# - item offset of action to take (I)
# - amount for action to take (d)
# - price for actio to take (d)
STRATEGIES_SCHEMA = struct.Struct("BIdBIdd")
# - the amount of the position
# - the cost basis of the position
POSITIONS_SCHEMA = struct.Struct("dd")

class RoyalItemSizer(object):
	"""
	Used to calculate and determine sizing of item markets.
	"""
	def __init__(self, items, orders, customers, strategies):
		self.header = MARKET_HEADER.size
		# Buying and Selling
		self.item = (ORDERS_SCHEMA.size * orders * 2) + ITEMS_SCHEMA.size
		self.customer = (STRATEGIES_SCHEMA.size * strategies) + (POSITIONS_SCHEMA.size * items)
		self.items = self.item * items
		self.customers = self.customer * customers
		self.total_size = self.items + self.customers + self.header



class RoyalItemMarket(object):
	
	def __init__(self, **kwargs):
		pass