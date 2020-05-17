// zte_vs.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <functional>
#include <string>
#include <ctime>

using namespace std;

using UI = unsigned int;
using USI = unsigned short int;
//保存图的数据
unordered_map<UI, vector<UI> > data_map, data_map_;
//给定循环上限和剪枝数
USI loop(14), cut_n(3);
//保存剪枝结果
int helper[5000];
//节点信息和路径
USI d[5000], path[14];
//保存查询出的环，用于去重
unordered_map<USI,set<string> > ress;
//节点数和结果
UI pd, p[12] = { 0 };
USI n_a(0), n_b(0);
//计时
clock_t t[5];
//输入文件和输出文件
string infile = "./Example.csv";
string outfile = "./result.txt";
//整数转字符串，用于去重和输出结果
std::string to_string_(int value) {

    static const char digits[19] = {
        '9','8','7','6','5','4','3','2','1','0',
        '1','2','3','4','5','6','7','8','9'
    };
    static const char* zero = digits + 9;//zero->'0'

    char buf[24];
    int i = value;
    char *p = buf + 24;
    *--p = '\0';
    do {
        int lsd = i % 10;
        i /= 10;
        *--p = zero[lsd];
    } while (i != 0);
    if (value <0)
        *--p = '-';
    return std::string(p);
}

//读取数据文件并处理
inline void file2matrix() {
    ifstream inFile(infile, ios::in);//inFile来自fstream,ifstream为输入文件流(从文件读入)
    string lineStr;
    vector<vector<string> > strArray;
    unordered_map<UI, vector<UI> > p_a;
    int i(0), j(0);
    //    getline(inFile,lineStr);
    //    cout<<lineStr;
    while (getline(inFile, lineStr)) //getline来自sstream
    {
        //打印整行字符串
        //        cout<<lineStr<<endl;
        //存成二维表结构
        stringstream ss(lineStr);//来自sstream
        string str;
        vector<string> lineArray;
        //按照逗号分隔
        j = 0;
        while (getline(ss, str, ',')) {
            if (str == "1" || str == "1\r")
                //                cout << str;
                p_a[i].push_back(j);
            //            lineArray.push_back(str);//一行数据以vector保存
            j++;
        }
        n_b = j;
        //cout<<lineArray[0]<<endl;
        //        strArray.push_back(lineArray);//每一行vector数据都放到strArray中去
        i++;
    }
    n_a = i;
    //    cout<<n_b;
    vector<UI>::iterator iter;
    for (i = 0; i<n_a; i++) {
        iter = p_a[i].begin();
        while (iter != p_a[i].end()) {
            data_map[i].push_back(*iter + n_a);
            data_map[*iter + n_a].push_back(i);
            data_map_[i].push_back(*iter + n_a);
            data_map_[*iter + n_a].push_back(i);
            iter++;
        }
    }
}
//整理数据排序
inline void data_sort() {
    auto iter = data_map.begin();
    while (iter != data_map.end()) {
        if (data_map_.count(iter->first)) {
            d[pd] = iter->first;
            sort(iter->second.begin(), iter->second.end());
            ++iter;
            pd++;
        }
        else {
            data_map.erase(iter++);
        }
    }
}
//剪枝
void search_helper(UI k, UI  key, UI cnt) {
    for (auto val : data_map_[k]) {
        if (data_map.count(val)) {
            helper[val] = key;
            if (cnt == cut_n) continue;
            search_helper(val, key, cnt + 1);
        }
        else continue;

    }
}
//深度搜索并结合剪枝数据拼接
void search_circle(USI path[], int cnt) {
    if (data_map.count(path[cnt - 1])) {
        for (auto& item : data_map[path[cnt - 1]]) {
            //如果超过循环数-剪枝数，根据剪枝的数据检查是否能成环，不能的话停止搜索
            if (cnt > loop - 1 - cut_n && (helper[item] != path[0] && helper[item] != -path[0] - 1)) continue;
            bool flag = false;
            if (cnt > 1) {
                for (int i = 0; i < cnt - 1; ++i)
                    if (path[i] == item) {
                        flag = true;
                        break;
                    }
            }
            if (flag) continue;
            //能成环，记录数据
            if (cnt > 1 && helper[item] == -path[0] - 1) {
                path[cnt] = item;
                //将环转换为字符串存入集合
                string line = "";
                USI path_copy[14];
                for(int j =0;j<=cnt;j++){
                    path_copy[j] = path[j];
                }
                sort(path_copy+0, path_copy+cnt+1, less<int16_t>());
                for (int i = 0; i <= cnt; i++) {
                    //将排序后的路径转化为字符串
                    if (i!=cnt)
                        line += to_string_(path_copy[i]) + ",";
                    else {
                        line += to_string_(path_copy[i]) + "\n";
                        //利用集合去重
                        ress[i+1].insert(line);
                    }
                }
                //                cout<<cnt+1<<":"<<p[cnt-2]<<endl;
            }
            if (cnt == loop - 1) continue;

            path[cnt] = item;
            search_circle(path, cnt + 1);
        }
    }
    else return;
}
//输出结果
void output(string &outputFile) {


    FILE *fp;
//    fopen_s(&fp, outputFile.c_str(), "w");
    fp = fopen(outputFile.c_str(), "w");
    string str = "Result:\n";
    //统计集合元素数量得到结果
    for (USI i = 4; i <= loop; i += 2) {
        p[i-3] = UI(ress[i].size());
        cout << "The number of circles with " << i << " items is " << p[i-3] << endl;
        str += to_string(i) + ":" + to_string(p[i - 3]) + "\n";
    }
    const char *p = str.c_str();
    fwrite(p, strlen(p), 1, fp);
    fclose(fp);
}

int main(int argc, char** argv) {
    t[0] = clock();
    //res11 = (USI*)mem_alloc(50000000000*14 * sizeof(USI));
    //res11 = (USI *)malloc(5000000 * 14 * sizeof(USI));
    cout << "Use ./zte_vs.exe [[number_of_loops] data_file_name]" << endl;
    cout << "eg1:" << "./zte_vs.exe" << endl;
    cout << "eg2:" << "./zte_vs.exe 10 " << endl;
    cout << "eg3:" << "./zte_vs.exe 12 Example6.csv" << endl;
    cout << "Default number_of_loops is 14" << endl;
    cout << "Default data_file_name is ./Example.csv" << endl;
    if (argc>1) {
        loop = USI(atoi(argv[1]));
    }
    if (argc>2) {
        infile = argv[2];
    }
    
    cout << "CSV data reading..." << endl;
    file2matrix();
    t[1] = clock();
    cout << "Time used:" << float(t[1] - t[0]) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Sorting data..." << endl;
    data_sort();
    sort(d, d + pd);
    t[2] = clock();
    cout << "Time used:" << float(t[2] - t[0]) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Searching circles...It may take long time at the begining.Please waiting..." << endl;

    for (int i = 0; i < n_a; i++) {
        cout << "Searching circles:" << float(100 * i) / n_a << "%" << endl;
        //得到剪枝用的数组
        search_helper(d[i], d[i], 1);
        //找d[i]节点的相邻节点
        for (auto item : data_map_[d[i]]) {
            if (data_map.count(item))
                helper[item] = -d[i] - 1;
        }
        path[0] = d[i];
        search_circle(path, 1);

        data_map.erase(d[i]);
    }
    cout << "Searching circle..." << "100%" << endl;
    t[3] = clock();
    cout << "Time used:" << float(t[3] - t[0]) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Clear duplicates..." << endl;
    output(outfile);
    //mem_free(res11, 50000000000 * 14 * sizeof(USI));
    t[4] = clock();
    cout << "Done! Results are saved in ./result.txt" << endl;
    cout << "Total time used:" << float(t[4] - t[0]) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
