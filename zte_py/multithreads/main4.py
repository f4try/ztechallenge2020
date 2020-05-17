import _thread
import copy
import random
import threading
import time

import pandas as pd
import numpy as np
from numba import jit
import networkx as nx
import matplotlib.pyplot as plt

# import pygraphviz as pgv


    # 4:62,6:1179,
# {4: 191, 6: 317, 8: 4362, 10: 0, 12: 0, 14: 0}
global result
result= {4: 0, 6: 0, 8: 0, 10: 0, 12: 0, 14: 0}
# 读入csv表格数据
data = pd.read_csv("Example1.csv")
# data.columns=range(data.shape[1])
# data.ix
# data.loc['256']=0
# data = data.loc[~(data==0).all(axis=1),:]
# data = data.loc[:,~(data==0).all(axis=0)]
# 生成a和b两个部落每个人的朋友关系数据
n_a = data.shape[0]
n_b = data.shape[1]
p_a = [np.argwhere(np.array(data.iloc[i]) > 0) for i in range(n_a)]
# p_b = [np.argwhere(np.array(data.iloc[:,i])>0) for i in range(n_b)]

G = nx.Graph()
G.add_nodes_from(range(n_a))
G.add_nodes_from(np.arange(n_b) + n_a)
for i in range(n_a):
    G.add_edges_from([(i, int(p) + n_a) for p in p_a[i]])

for i in list(G.nodes):
    if G.degree[i] < 2:
        print(i)
        G.remove_node(i)
print(G.number_of_nodes())

# path_set = []
if n_a < n_b:
    nodes = range(n_a)
else:
    nodes = range(n_a, n_a + n_b)

def myCompute(label, n):
    for i in nodes:
        if i%n == label:
            for j in G.adj[i]:
                paths = list(nx.all_simple_paths(G, i, j, 12))
                for path in paths:
                    l = len(path)
                    if l > 2:
                        result[l] += 1
            print(i)



# 创建两个线程
# try:
#    _thread.start_new_thread(myCompute(0, 2), ("Thread-1", 2, ) )
#    _thread.start_new_thread(myCompute(1, 2), ("Thread-2", 4, ) )
# except:
#    print ("Error: 无法启动线程")
#
# while 1:
#    pass


start_time = time.time()
threads = []
# t1 = threading.Thread(target=myCompute(0, 4))
# threads.append(t1)
# t2 = threading.Thread(target=myCompute(1, 4))
# threads.append(t2)
# t3 = threading.Thread(target=myCompute(2, 4))
# threads.append(t3)
t4 = threading.Thread(target=myCompute(3, 4))
threads.append(t4)
for t in threads:
        t.setDaemon(True)
        t.start()
for i in result:
    result[i] = result[i] / i
print(result)
elapsed = (time.time() - start_time)
print("Time used:", elapsed)