

"""
Tools to validate schema of python objects
"""

class RoyalValidationError(Exception):
	pass


class SchemaValidator(object):

	def __init__(self, **kwargs):
		self.schema = kwargs
		self.checks = []

	def validate(self, obj):
		for key in self.schema:
			if key not in obj:
				raise RoyalValidationError("Expected key '" + key + "' in object.")
			if isinstance(self.schema[key], type) and not isinstance(obj[key], self.schema[key]):
				raise RoyalValidationError("Expected key '" + key + "' to be of type: " + str(self.schema[key]))

		for check in self.checks:
			if not check(obj):
				raise RoyalValidationError("Object does not pass all checks")

	def add_check(self, check):
		self.checks.append(check)