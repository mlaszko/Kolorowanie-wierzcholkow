import sys
from random import randrange

print sys.argv[1]
print sys.argv[2]

file = open(sys.argv[3], 'w')
n = int(sys.argv[1])
p = int(sys.argv[2])
file.write(sys.argv[1])
file.write('\n')

tab = [[0 for col in range(n)] for row in range(n)]

for i in range(n):
	for j in range(i,n):
		if i==j:
			tab[i][j]='0 '
		else:
			r = randrange(100)
			if(r < p):
				tab[i][j] = tab[j][i] = '1 '
			else:
				tab[i][j] = tab[j][i] = '0 '


for i in range(n):
	for j in range(n):
		file.write(tab[i][j])
	file.write('\n')


file.close()
