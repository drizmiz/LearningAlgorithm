// LearningAlgorithms_Cpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

//#include "BagQueueAndStack.h"
#include "Tester.h"
#include "Sort.h"
#include "Search.h"
#include "Graph.h"
//#include "DynamicProgramming.h"
//#include "Prime.h"

//#define __ReadFile
//#define __DynamicProgramming
//#define __SubString
//#define __Prime
//#define __Sort

#define test(F) t.start(); \
				F; \
				t.end(); \
				std::cout << "数据处理时间：" << t.timespan() << "ms" << std::endl;

#define callAndTest(object, F) \
		Sort<int> object = Sort<int>(vecs[vecindex].begin(), vecs[vecindex].end() - vecs[vecindex].begin()); \
		test(object.sort(Sort<int>::methods::F)); \
		cout << "same_with_std: " << boolalpha << (vecs[0] == vecs[vecindex]) << endl; \
		vecindex++;

int comp(const void* a, const void* b)
{
	return *(int*)a < *(int*)b;
}

template<typename T>
bool __less(std::vector<T>& a, std::vector<T>& b, unsigned& place)
{
	for (unsigned i = 0; i < a.size(); i++)
	{
		if (a[i] < b[i])
		{
			place = a[i];
			return true;
		}
		else if (a[i] > b[i])
			return false;
	}
	return false;
}

using namespace std;
// 测试用例
int main()
{
	typedef unsigned uint;
	//cout.setf(ios::unitbuf);
#ifndef __ReadFile
	const string path = "C:\\Users\\drizzle\\source\\repos\\algs4-data\\";
	string filename;
	cin >> filename;
	ifstream input(path + filename);
	if (!input.is_open())
		throw exception("File not open!");
#endif
	/* Stack 用例 */
#ifdef __MyStack
	RADeque<string> s;
	string tmp;
	try {
		while (input >> tmp)
		{
			if (tmp != "-")
				s.push_front(tmp);
			else if (!s.isEmpty())
				cout << s.pop_front() << " ";
		}
	}
	catch (exception e)
	{
		cerr << endl << e.what();
	}
	cout << endl << s.currentSize() << "left" << endl;
	for (string a : s)
		cout << a << " ";
#endif
	/* Queue 用例 */
#ifdef __MyQueue
	Deque<string> s;
	string tmp;
	while (input >> tmp)
	{
		if (tmp != "-")
			s.pushLeft(tmp);
		else if (!s.isEmpty())
			cout << s.popRight() << " ";
	}
	cout << endl << s.currentSize() << "left";
#endif
	/* 演示程序 ThreeSum */
#ifdef __ThreeSum
	RADeque<int> a;
	int tmp;
	Timer t;
	t.start();
	while (input >> tmp)
		a.push_back(tmp);
	t.end();
	cout << "数据读入及存储时间：" << t.timespan() << "ms" << endl;
	t.start();
	int cnt = ThreeSum::count(a);
	t.end();
	cout << "数据处理时间：" << t.timespan() << "ms" << endl;
	cout << "count=" << cnt;
#endif
#ifdef __DynamicProgramming
	using namespace DynamicProgramming;
#ifdef __Triangle
	Timer t;
	t.start();
	triangle::input();
	t.end();
	cout << "数据读入及存储时间：" << t.timespan() << "ms" << endl;
	t.start();
	cout << triangle::MaxSum_DP1(0, 0) << endl;
	t.end();
	cout << "数据处理时间：" << t.timespan() << "ms" << endl;
	t.start();
	cout << triangle::MaxSum_DP2(0, 0) << endl;
	t.end();
	cout << "数据处理时间：" << t.timespan() << "ms" << endl;
#endif
#ifdef __SubString
	cout << sub_string::max_length_of_sub_str("abcdefg", "zbdegloa");
#endif
#endif
#ifdef __Prime
	const int N = static_cast<int>(5e+7);
	vector<int> prime;
	Timer t;
	//cout << CComparer::cvalue(findPrime_E, N) << endl;
	//cout << CComparer::cvalue(find_prime_trivial, N) << endl;
	//t.start();
	//prime = findPrime_A(N);
	//t.end();
	//for (int i : prime)
		//cout << i << " ";
	//cout << endl << "平凡筛数据处理时间：" << t.timespan() << "ms" << endl;
	t.start();
	prime = findPrime_E(N);
	t.end();
	//for (int i : prime)
		//cout << i << " ";
	cout << endl << "线性筛（？）数据处理时间：" << t.timespan() << "ms" << endl;
	t.start();
	prime = find_prime_trivial(N);
	t.end();
	//for (int i : prime)
		//cout << i << " ";
	cout << endl << "更好的线性筛数据处理时间：" << t.timespan() << "ms" << endl;
	//system("pause");
#endif
#ifdef __Sort
	vector<int> vec;
	vector<vector<int>> vecs;
	const int N = (int)1e7;
	const int methodsCnt = 5;
	Timer t;
	srand(0);
	for (int i = 0; i < N; i++)
		vec.push_back(rand()*rand());
	for (int i = 0; i < methodsCnt; i++)
		vecs.push_back(vec);
	int vecindex = 0;
	string s;
	cin >> s;
	std::reverse(s.begin(),s.end());
	cout << s;
	try {
		cout << "For N = " << N << endl;
		cout << "std::sort: ";
		test(std::sort(vecs[vecindex].begin(), vecs[vecindex].end()));
		vecindex++;
		cout << "Merge: ";
		callAndTest(a, Merge_T2B);
		//a.outl();
		cout << "Quick: ";
		callAndTest(b, Quick);
		//b.outl();
		auto qtime3way = t.timespan();
		cout << "Quick_3way: ";
		callAndTest(c, Quick_3way);
		//c.outl();
		auto qtime3wayfast = t.timespan();
		cout << (long double)qtime3way / qtime3wayfast << " times faster!" << endl;
		cout << "Shell: ";
		callAndTest(d, Shell);
		//d.outl();
		//cout << "qsort: ";
		//test(qsort(vecs[vecindex].data(), vecs[vecindex].size(), sizeof(int), comp));
		//unsigned p = 65536;
		//cout << boolalpha << __less(vecs[2], vecs[1], p) << p << __less(vecs[1], vecs[2], p) << p;
	}
	catch (exception e) {
		cerr << endl << "Error: " << e.what() << endl;
	}
	//d.outl();
#endif
	cout << "Processing..." << endl;
	Graph<uint> graph(input);
	cout << "Graph Processed!" << endl;
	uint source;
	cin >> source;
	cout << "Searching..." << endl;
	Graph<uint>::breadth_first_search bfs(graph, source);
	cout << "Searching Done!" << endl;
	ios_base::sync_with_stdio(false);
	for (uint i = 0; i < graph.p_size(); ++i)
	{
		vector<uint> path = bfs.shortest_path_to(i);
		cout << "the shortest path to " << i << " is:";
		for (uint j : path)
			cout << " " << j;
		cout << endl;
	}
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
