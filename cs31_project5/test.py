cnt = 0
for i in range(4 + 1):
    for j in range(5 + 1):
        for k in range(8 + 1):
            for w in range(9 + 1):
                if i + j + k + w == 12:
                    cnt += 1
print(cnt)
