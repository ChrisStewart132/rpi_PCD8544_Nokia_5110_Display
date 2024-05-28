fname = input("filename: ")
f = open(fname, 'rb')

output = ""

byte = f.read(1)
while byte:
	output += str(int(byte == b'\x01'))
	byte = f.read(1)
print()
print(output)