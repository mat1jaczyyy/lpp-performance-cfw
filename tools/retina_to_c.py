_r, _g, _b = [], [], []

try:
    while True:
        x = input().split()
        _r.append(x[1])
        _g.append(x[2])
        _b.append(x[3][:-1])
		
except EOFError:
    pass

print("{" + ', '.join(str(i) for i in _r) + "},")
print("{" + ', '.join(str(i) for i in _g) + "},")
print("{" + ', '.join(str(i) for i in _b) + "},")