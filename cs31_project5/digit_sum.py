cnt = 0
for i in range(0, 1000000):
    if sum(int(i) for i in str(i)) == 20:
        cnt += 1

print(cnt)
