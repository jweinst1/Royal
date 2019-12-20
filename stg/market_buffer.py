import struct
import sys

# Script prototype of buffer construction for a market

def make_buffer(size):
	return bytearray([0 for i in range(size)])

def make_sized_buf(form, n):
	return bytearray([0 for i in range(n * struct.calcsize(form))])

class OrderBuffer(object):
	form = "IBdd"
	form_size = struct.calcsize(form)

	def __init__(self, count):
		self.count = count
		self.total_size = OrderBuffer.form_size * 2 * count
		self.buy_offset = 0
		self.sell_offset = OrderBuffer.form_size * count
		self.buf = make_buffer(OrderBuffer.form_size * 2 * count)
	def __len__(self):
		return self.total_size

	def change(self, **kwargs):
		if 'at' not in kwargs:
			raise KeyError("Must have 'at' in call to change")
		i = kwargs['at']
		ioff = i * OrderBuffer.form_size
		got = struct.unpack_from(OrderBuffer.form, self.buf, ioff)
		newargs = [0, 0, 0.0, 0.0]
		# Either keep same values or change values
		newargs[0] = kwargs.get('cust', got[0])
		newargs[1] = kwargs.get('type', got[1])
		newargs[2] = kwargs.get('amount', got[2])
		newargs[3] = kwargs.get('price', got[3])
		struct.pack_into(OrderBuffer.form, self.buf, ioff, *newargs)


	def __repr__(self):
		base_str = "Buys: \n"
		i = 0
		while i < self.sell_offset:
			retr = struct.unpack_from(OrderBuffer.form ,self.buf, i)
			base_str += " Customer: " + str(retr[0])
			base_str += " Type: " + str(retr[1])
			base_str += " Amount: " + str(retr[2])
			base_str += " Price: $" + str(retr[3]) + "\n"
			i += OrderBuffer.form_size
		base_str += "Sells: \n"
		while i < self.total_size:
			retr = struct.unpack_from(OrderBuffer.form ,self.buf, i)
			base_str += " Customer: " + str(retr[0])
			base_str += " Type: " + str(retr[1])
			base_str += " Amount: " + str(retr[2])
			base_str += " Price: $" + str(retr[3]) + "\n"
			i += OrderBuffer.form_size
		base_str += "---------------------------------------------"
		return base_str

if __name__ == '__main__':
	a = OrderBuffer(5)
	print(a)
	a.change(at=1, price=5.5)
	print(a)