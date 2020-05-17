import collections
import copy
import csv
import time
import sys


loops = 8
start_time = time.time()
data = []
p_a = []
# with open(sys.argv[1])as f:
with open('Example4.csv')as f:
    f_csv = csv.reader(f)
    for row in f_csv:
        temp =[]
        data.append(row)
        for i in range(len(row)):
            if int(row[i])>0:
                temp.append(i)
        p_a.append(temp)
n_a = len(data)
n_b = len(data[0])
# p_b = [np.argwhere(np.array(data.iloc[:,i])>0) for i in range(n_b)]

class Graph():
    def __init__(self, vertices):
        self.graph = collections.defaultdict(list)
        self.V = vertices

    def addEdge(self, u, v):
        self.graph[u].append(v)

G = Graph(n_a + n_b)
for i in range(n_a):
    for p in p_a[i]:
        G.addEdge(i, int(p) + n_a)
        G.addEdge(int(p) + n_a, i )

if n_a <= n_b:
    nodes = range(n_a)
else:
    nodes = range(n_a, n_a + n_b)

def all_simple_paths(G, source, targets, cutoff):
    visited = collections.OrderedDict.fromkeys([source])
    stack = [iter(G.graph[source])]
    targets = set(targets)
    while stack:
        children = stack[-1]
        child = next(children, None)
        if child is None:
            stack.pop()
            visited.popitem()
        elif len(visited) < cutoff:
            if child in visited:
                continue
            if child in targets:
                yield list(visited) + [child]
            visited[child] = None
            if targets - set(visited.keys()):
                stack.append(iter(G.graph[child]))
            else:
                visited.popitem()
        else:
            for target in (targets & (set(children) | {child})) - set(visited.keys()):
                yield list(visited) + [target]
            stack.pop()
            visited.popitem()

result= {4: 0, 6: 0, 8: 0, 10: 0, 12: 0, 14: 0}
g_i = []
# f = open('result.txt', 'w')
for i in nodes:
    if len(G.graph[i]) > 1:
        paths = list(all_simple_paths(G, i, G.graph[i], loops))
        for path in paths:
            l = len(path)
            path.sort()
            if l > 2 and path not in g_i:
                g_i.append(path)
                result[l] += 1
        print(i)
#     print(result)
#     f.write(str(i)+'\n')
#     f.write(str(result)+'\n')
# f.close()
print(result)
elapsed = (time.time() - start_time)
print("Time used:", elapsed)
