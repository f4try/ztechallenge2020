import time


def deep_search(map_record,path,result,cnt, visited):
    if cnt > loop_n:
        return
    node = path[-1]
    if node in map_record:
        for each_node in sorted(map_record[node]):
            if each_node == path[0]:
                if cnt > 2:
                    result[cnt].append(path)
                    # result[(cnt-4)/2].append(path)
                    # result[cnt]+=1
            elif each_node > path[0] and each_node not in path:
                if cnt > int(loop_n/2) and each_node not in visited[path[0]]:
                    continue
                deep_search(map_record,list(path)+[each_node],result,cnt + 1, visited)
    return
def search_all(map_record,visited):
    for key in map_record.keys():
        que = set()
        que.add(key)
        for i in range(int(loop_n/2)):
            que_new = set()
            for node in que:
                # if node in map_record:
                for each_node in sorted(map_record[node]):
                    que_new.add(each_node)
                    visited[each_node].add(key)
            que = que_new
            # print(key, que)
def test(loop_n = 10):
    start_time = time.time()
    data = open("Example.csv", "r")
    map_record = {}
    visited = {}
    # p_a = defaultdict(list)
    p_a ={}
    i=0
    n_b=0
    for row_str in data:
        p_a[i] = []
        row = row_str.split(",")
        n_b = len(row)
        for j in range(n_b):
            if int(row[j]) > 0:
                # p_a[i].add(j)
                p_a[i].append(j)
        i+=1

    n_a = i
    for i in range(n_a+n_b):
        map_record[i] = set()
        visited[i] = set()
    for p1,p2s in p_a.items():
            for p2 in p2s:
                map_record[p1].add(p2+n_a)
                map_record[p2+n_a].add(p1)
    result = {}
    # result = []
    result_n = {}
    result_sorted = {}
    for i in range(4,loop_n+1,2):
        result[i] = []
        # result.append([])
        result_sorted[i] = []
        result_n[i] = 0
    search_all(map_record,visited)
    # print(visited)
    for key in sorted(map_record.keys()):
        print(key)
        deep_search(map_record,[key],result,1, visited)

    # sum_len = 0

    for i in range(4,loop_n+1,2):
        for path in result[i]:
            temp = sorted(path)
            if temp not in result_sorted[i]:
                result_sorted[i].append(temp)
                result_n[i] += 1
        # sum_len = sum_len + len(result[i])
    # print(result)
    print(result_n)
    #
    #file = open("./result1.txt", "w")
    # file = open("result.txt", "w")
    # file.write(str(result_n)+"\n")
    # for i in range(4,loop_n+1,2):
    #     for each_result in result_sorted[i]:
    #         # if i ==14:
    #         for j in range(len(each_result)):
    #             if(j == 0):
    #                 file.write(str(each_result[j]))
    #             else:
    #                 file.write(","+str(each_result[j]))
    #         file.write("\n")
    # file.close()
    elapsed = (time.time() - start_time)
    print("Time used:", elapsed)