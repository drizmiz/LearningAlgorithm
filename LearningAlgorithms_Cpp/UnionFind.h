/*
BQS_H
*/

#pragma once

#ifndef UF_H
#define UF_H

class UnionFind
{
	using node = int;
private:
	int* id;
	int cnt;
public:
	UnionFind(int N) :id(new int[N]), cnt(N)
	{
		for (int i = 0; i < N; i++)
			id[i] = i;
	}
	~UnionFind() { delete[]id; }
	int count() { return this->cnt; }
	int find(node);
	bool connected(node p, node q)
	{
		return find(p) == find(q);
	}
	void _union(node p, node q);
};

#endif
