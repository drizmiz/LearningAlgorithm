
#pragma once

#ifndef PRIME_H
#define PRIME_H

#include <vector>
#include <cmath>
#include "Tester.h"

bool is_prime(int N)
{
	for (int i = 2; i <= std::sqrt(N); i++)
		if (N % i == 0)
			return false;
	return true;
}
std::vector<int> find_prime_trivial(int N)
{
	std::vector<int> Prime;
	for (int i = 2; i <= N; i++)
		if (is_prime(i))
			Prime.push_back(i);
	return Prime;
}
std::vector<int> find_prime_yrq(int N)
{
	std::vector<int> primes;
	primes.reserve(N / 10);
	auto visited = (char*)malloc(N + 1);
	if (visited == nullptr)
		throw std::exception("bad_alloc");
	memset(visited, 0, N + 1);
	if (N <= 2)
		return primes;
	for (int i = 2; i < N; ++i) {
		if (!visited[i])
			primes.push_back(i);
		for (auto beg = primes.begin(); beg != primes.end() && i * *beg <= N;
			++beg) {
			visited[i * *beg] = 1;
			if (!(i % *beg))
				break;
		}
	}
	return primes;
}
std::vector<int> findPrime_A(int N)
{
	std::vector<int> Prime;
	if (N <= 1)return Prime;
	if (N == 2)
	{
		Prime.push_back(2);
		return Prime;
	}
	Prime.reserve(N / std::log((double)N));
	int* numbers = new int[N + 1];
	for (int i = 0; i <= N; i++)
		numbers[i] = i;
	for (int i = 2; i <= std::sqrt(N); i++)
	{
		if (numbers[i] == -1) continue;
		for (int j = i; i * j <= N; j++)
			numbers[i * j] = -1;
	}
	for (int i = 2; i <= N; i++)
		if (numbers[i] != -1)
			Prime.push_back(numbers[i]);
	delete[] numbers;
	return Prime;
}

template<class Int = __int64>
std::vector<Int> findPrime_E(Int N)
{
	std::vector<Int> Prime;
	if (N <= 1)return Prime;
	if (N == 2)
	{
		Prime.push_back(2);
		return Prime;
	}
	Prime.reserve(N / std::log((double)N));
	Int* primes = new Int[N + 1];
	for (int i = 1; i <= N; i++)
		primes[i] = i;
	for (Int p = 2; p <= std::sqrt(N); p++)
	{
		if (!primes[p]) continue;
		for (int q = p; (Int)p * q <= N; q++)
		{
			if (!primes[q])continue;
			int i = q;
			for (;;)
			{
				Int _p = p * i;
				if (_p > N)break;
				primes[_p] = 0;
				i = _p;
			}
		}
	}
	for (int i = 2; i <= N; i++)
		if (primes[i] != 0)
			Prime.push_back(primes[i]);
	delete[] primes;
	return Prime;
}

#endif