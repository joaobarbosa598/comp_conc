from typing import Tuple
import numpy as np

_RANGE = 25
option_text = """Initialize with specific or random values?
1 - Specific
2 - Random
"""

def get_dimension() -> Tuple[int, int]:
	l = input("Lines: ")
	c = input("Columns: ")
	return int(l), int(c)

def get_init_option():
	r = input(option_text)
	if int(r) == 2:
		return None # random values
	else:
		v = input("Value: ")
		return v # specific value
	
if __name__ == '__main__':

	dimension = get_dimension()
	option = get_init_option()
	
	if option == None:
		m = np.random.rand(dimension[0], dimension[1]) * _RANGE
	else:
		m = np.full(dimension, option, dtype = float)
	
	f = input("Filename: ")
	np.savetxt(f, m, fmt = "%s")
	
	with open(f, 'r+') as file:
		content = file.read()
		file.seek(0, 0)
		s = str(dimension[0]) + ' ' + str(dimension[1]) 
		file.write(s + '\n' + content)

