import struct
from .schema_tools import SchemaValidator, SchemaValidationError

# - Size of the market (I)
# - Number of turns (L)
# - number of customers (I)
# - number of items (I)
# - max orders (I)
# - max strategies (I)
# - state of market (order or exec) (B)
# - state of item order execution (i, off is -1)
# - state of strategy execution for customers (i, off is -1)
# - votality of the market (d)
MARKET_HEADER = struct.Struct("ILIIIIBiid")

# - current price (d)
# - current order state (i, off is -1), used for exec or
#     placing orders, when an order queue is full.
# - String name of item (32s, last must be \0)
ITEMS_SCHEMA = struct.Struct("di32s")
# - type of order (B)
# - offset of customer (I)
# - amount ordering (d)
# - price ordering (d)
# ~ Note: buy chunk of orders comes before sell chunk
ORDERS_SCHEMA = struct.Struct("BIdd")
# - The cash possessed by the customer (d)
# - current strategy state (i, off is -1)
# - String name of customer (32s, last must be \0)
CUSTOMERS_SCHEMA = struct.Struct("di32s")
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

OBJECT_MARKET_SCHEMAS = {
	'customer':SchemaValidator(name=str, cash=float),
	'item':SchemaValidator(name=str, price=float),
	'market':SchemaValidator(items=int, orders=int, customers=int, strategies=int)
}

class RoyalItemError(Exception):
	pass

class RoyalItemSizer(object):
	"""
	Used to calculate and determine sizing of item markets.
	"""
	def __init__(self, items, orders, customers, strategies):
		if 0 in (items, orders, customers, strategies):
			raise RoyalItemError("Market construction arguments cannot be 0")
		self.header = MARKET_HEADER.size
		# Buying and Selling
		self.item = (ORDERS_SCHEMA.size * orders * 2) + ITEMS_SCHEMA.size
		self.customer = (STRATEGIES_SCHEMA.size * strategies) + (POSITIONS_SCHEMA.size * items) + CUSTOMERS_SCHEMA.size
		self.items = self.item * items
		self.customers = self.customer * customers
		self.total_size = self.items + self.customers + self.header



class RoyalItemMarket(object):
	
	def __init__(self, **kwargs):
		OBJECT_MARKET_SCHEMAS['market'].validate(kwargs)
		self.item_count = kwargs['items']
		self.order_count = kwargs['orders']
		self.customer_count = kwargs['customers']
		self.strategy_count = kwargs['strategies']
		self.votality = kwargs.get('votality', 1.0)
		self.sizer = RoyalItemSizer(self.item_count, self.order_count, self.customer_count, self.strategy_count)
		self.buf = bytearray([0 for i in range(self.sizer.total_size)])
		MARKET_HEADER.pack_into(self.buf, 0, 
			                    self.sizer.total_size,
			                    0, 
			                    self.customer_count, self.item_count,
			                    self.order_count, self.strategy_count,
			                    0, 
			                    -1, 0,
			                    self.votality)

	def __repr__(self):
		market_info = MARKET_HEADER.unpack_from(self.buf, 0)
		base_str = ""
		base_str += "RoyalItemMarket - size:" + str(market_info[0])
		base_str += ", turns: " + str(market_info[1])
		base_str += ", customers:" + str(market_info[2])
		base_str += ", items:" + str(market_info[3])
		base_str += ", max orders:" + str(market_info[4])
		base_str += ", max strategies:" + str(market_info[5])
		base_str += "\n"
		return base_str

	def __len__(self):
		return len(self.buf)

class RoyalItemBuilder(object):

	def __init__(self):
		self.items = {}
		self.customers = {}
		self.votality = 1.0
		self.max_orders = 20
		self.max_strategies = 20
		self.market = None

	def add_item(self, *args):
		for arg in args:
			OBJECT_MARKET_SCHEMAS['item'].validate(arg)
			self.items.update(arg)

	def add_customer(self, *args):
		for arg in args:
			OBJECT_MARKET_SCHEMAS['customer'].validate(arg)
			self.customers.update(arg)

