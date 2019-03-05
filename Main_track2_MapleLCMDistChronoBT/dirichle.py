#!/usr/bin/env python3

import subprocess
import random
import sys

n = int(sys.argv[1])
fileName = 'n' + str(n) + '.cnf'
f = open(fileName, 'w')
sum = 0
a = n
while(a > 0):
	sum = sum + a
	a = a - 1
header = 'p cnf ' + str((n + 1) * n) + ' ' + str(sum * n + n + 1)
f.write(header + '\n')
count = 1
x = [[0 for j in range(0, n+3)] for i in range(0, n+2)]
for i in range(1, n + 2):
	for j in range(1, n + 1):
		x[i][j] = count
		f.write(str(x[i][j]))
		f.write(' ')
		count += 1
	f.write('0\n')
for k in range(1, n + 1):
	for j in range(1, n + 1):
		for i in range(j + 1, n + 2):
			f.write('-' + str(x[j][k]) + ' -'+ str(x[i][k]) + ' 0\n')
f.close()