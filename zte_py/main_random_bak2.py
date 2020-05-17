import copy
import random
import pandas as pd
import numpy as np
from numba import jit

# 4:62,6:1179,

# 读入csv表格数据
data = pd.read_csv("Example0.csv")
# data.columns=range(data.shape[1])

# 生成a和b两个部落每个人的朋友关系数据
p_a = [np.argwhere(np.array(data.iloc[i])>0) for i in range(data.shape[0])]
p_b = [np.argwhere(np.array(data.iloc[:,i])>0) for i in range(data.shape[1])]

#进行14次祭品传递
g = []
g_origin=[]
gn = 0
tor = 0
g_result = {4: 0, 6: 0, 8: 0, 10: 0, 12: 0, 14: 0}
while tor < 1000:
    g_i = []
    for i in range(4):
        if i ==0:
            label = random.randint(0, 1)
            p = random.randint(0, data.shape[label]-1)
        else:
            label = 1 - label
            if label == 1:
                p = p_a[g_i[-1]][random.randint(0, p_a[g_i[-1]].size-1)]
            else:
                p = p_b[g_i[-1]][random.randint(0, p_b[g_i[-1]].size-1)]
        g_i.append(int(p))
        if g_i.index(p) != i:
            break
    # print(g_i)

    if (len(g_i) >= 4) and (g_i[0] == g_i[-1]):
        g_i_origin = copy.copy(g_i)
        g_i.sort()
        if g_i not in g:
            gn += 1
            g_origin.append(g_i_origin)
            g.append(g_i)
            tor = 0
            l = len(g_i_origin)
            if l % 2 == 0:
                g_result[l] += 1
            print(g_result)
        else:
            print("*******************************************")
            tor +=1

print(g_origin)
print(gn)
print(g_result)
# g_result={4:0,6:0,8:0,10:0,12:0,14:0}
# for i in g:
#     l = len(i)
#     if l%2 ==0:
#         g_result[l]+=1
# print(g_result)