import math

def nCr(n, r):
  f = math.factorial
  return f(n) / f(r) / f(n - r)

n = 14
sum = 0
for i in range(n):
  if i == 0:
    continue
  sum += nCr(n, i) * (math.pow(2, n) - math.pow(2, i) - math.pow(2, n - i) + 1)

print sum / math.pow(2, n)
